#include "netlist/persistent/netlist_serializer.h"
#include "netlist/gate_library/gate_library_manager.h"
#include "netlist/netlist.h"
#include "netlist/netlist_factory.h"
#include "test_def.h"
#include "gtest/gtest.h"
#include <boost/filesystem.hpp>
#include <core/log.h>
#include <core/utils.h>
#include <iostream>
#include <netlist/gate.h>
#include <netlist/net.h>
#include <netlist/module.h>

class netlist_serializer_test : public ::testing::Test
{
protected:
    const std::string g_lib_name = "EXAMPLE_GATE_LIBRARY";
    hal::path test_hal_file_path;

    virtual void SetUp()
    {
        NO_COUT_BLOCK;
        gate_library_manager::load_all();
        test_hal_file_path = core_utils::get_binary_directory() / "tmp.hal";
    }

    virtual void TearDown()
    {
        //boost::filesystem::remove(test_hal_file_path);
    }

    // Creates an empty netlist with a certain id if passed
    std::shared_ptr<netlist> create_empty_netlist(const int id = -1)
    {
        NO_COUT_BLOCK;
        std::shared_ptr<gate_library> gl = gate_library_manager::get_gate_library(g_lib_name);
        std::shared_ptr<netlist> nl(new netlist(gl));

        if (id >= 0)
        {
            nl->set_id(id);
        }
        return nl;
    }

    /*
     *      Example netlist circuit diagram (Id in brackets). Used for get fan in and
     *      out nets.
     *
     *
     *      GND (1) =-= INV (3) =--=             ------= INV (4) =
     *                                        AND2 (0) =-
     *      VCC (2) =----------------=             ------=
     *                                                            AND2 (5) =
     *                                                          =
     *
     *     =                       =               =----------=                =
     *       BUF (6)      ... OR2 (7)          ... OR2 (8)
     *     =                       =               =          =                =
     */

    // Creates a simple netlist shown in the diagram above
    std::shared_ptr<netlist> create_example_netlist(int id = -1)
    {
        NO_COUT_BLOCK;
        std::shared_ptr<gate_library> gl = gate_library_manager::get_gate_library(g_lib_name);
        std::shared_ptr<netlist> nl      = std::make_shared<netlist>(gl);
        if (id >= 0)
        {
            nl->set_id(id);
        }

        // Create the gates
        std::shared_ptr<gate> gate_0 = nl->create_gate(1, "AND2", "gate_0");
        std::shared_ptr<gate> gate_1 = nl->create_gate(2, "GND", "gate_1");
        std::shared_ptr<gate> gate_2 = nl->create_gate(3, "VCC", "gate_2");
        std::shared_ptr<gate> gate_3 = nl->create_gate(4, "INV", "gate_3");
        std::shared_ptr<gate> gate_4 = nl->create_gate(5, "INV", "gate_4");
        std::shared_ptr<gate> gate_5 = nl->create_gate(6, "AND2", "gate_5");
        std::shared_ptr<gate> gate_6 = nl->create_gate(7, "BUF", "gate_6");
        std::shared_ptr<gate> gate_7 = nl->create_gate(8, "OR2", "gate_7");
        std::shared_ptr<gate> gate_8 = nl->create_gate(9, "OR2", "gate_8");

        // Add the nets (net_x_y1_y2... := net between the gate with id x and the gates y1,y2,...)
        std::shared_ptr<net> net_1_3 = nl->create_net(13, "net_1_3");
        net_1_3->set_src(gate_1, "O");
        net_1_3->add_dst(gate_3, "I");

        std::shared_ptr<net> net_3_0 = nl->create_net(30, "net_3_0");
        net_3_0->set_src(gate_3, "O");
        net_3_0->add_dst(gate_0, "I0");

        std::shared_ptr<net> net_2_0 = nl->create_net(20, "net_2_0");
        net_2_0->set_src(gate_2, "O");
        net_2_0->add_dst(gate_0, "I1");

        std::shared_ptr<net> net_0_4_5 = nl->create_net(045, "net_0_4_5");
        net_0_4_5->set_src(gate_0, "O");
        net_0_4_5->add_dst(gate_4, "I");
        net_0_4_5->add_dst(gate_5, "I0");

        std::shared_ptr<net> net_7_8 = nl->create_net(78, "net_7_8");
        net_7_8->set_src(gate_7, "O");
        net_7_8->add_dst(gate_8, "I0");

        return nl;
    }

    // Checks if two vectors have the same content regardless of their order
    template<typename T>
    bool vectors_have_same_content(std::vector<T> vec_1, std::vector<T> vec_2)
    {
        if (vec_1.size() != vec_2.size())
            return false;

        // Each element of vec_1 must be found in vec_2
        while (vec_1.size() > 0)
        {
            auto it_1       = vec_1.begin();
            bool found_elem = false;
            for (auto it_2 = vec_2.begin(); it_2 != vec_2.end(); it_2++)
            {
                if (*it_1 == *it_2)
                {
                    found_elem = true;
                    vec_2.erase(it_2);
                    break;
                }
            }
            if (!found_elem)
            {
                return false;
            }
            vec_1.erase(it_1);
        }

        return true;
    }

    endpoint get_dst_by_pin_type(std::vector<endpoint> dsts, std::string pin_type)
    {
        for (auto dst : dsts)
        {
            if (dst.get_pin_type() == pin_type)
            {
                return dst;
            }
        }
        return {nullptr, ""};
    }

    // Returns true if two nets have the same content
    bool nets_are_equal(std::shared_ptr<net> n0, std::shared_ptr<net> n1)
    {
        if (n0 == nullptr || n1 == nullptr)
        {
            if (n0 == n1)
                return true;
            else
                return false;
        }
        if (n0->get_id() != n1->get_id())
            return false;
        if (n0->get_name() != n1->get_name())
            return false;
        if (n0->get_src().get_pin_type() != n1->get_src().get_pin_type())
            return false;
        if (!gates_are_equal(n0->get_src().get_gate(), n1->get_src().get_gate()))
            return false;
        for (auto n0_dst : n0->get_dsts())
        {
            if (!gates_are_equal(n0_dst.get_gate(), get_dst_by_pin_type(n1->get_dsts(), n0_dst.get_pin_type()).get_gate()))
            {
                return false;
            }
        }
        if (n0->get_data() != n1->get_data())
            return false;
        return true;
    }

    // Returns true if the two gates have the same content TODO: decorator types
    bool gates_are_equal(std::shared_ptr<gate> g0, std::shared_ptr<gate> g1)
    {
        if (g0 == nullptr || g1 == nullptr)
        {
            if (g0 == g1)
                return true;
            else
                return false;
        }
        if (g0->get_id() != g1->get_id())
            return false;
        if (g0->get_name() != g1->get_name())
            return false;
        if (g0->get_type() != g1->get_type())
            return false;
        if (g0->get_data() != g1->get_data())
            return false;
        return true;
    }

    // Returns true if the two modules have the same content
    bool modules_are_equal(std::shared_ptr<module> sm0, std::shared_ptr<module> sm1)
    {
        if (sm0 == nullptr || sm1 == nullptr)
        {
            if (sm0 == sm1)
                return true;
            else
                return false;
        }
        if (sm0->get_id() != sm1->get_id())
            return false;
        if (sm0->get_name() != sm1->get_name())
            return false;
        if (sm0->get_data() != sm1->get_data())
            return false;

        // Check if gates and nets are the same
        if (sm0->get_gates().size() != sm1->get_gates().size())
            return false;
        for (auto g_0 : sm0->get_gates())
        {
            std::shared_ptr<gate> g_1 = sm1->get_netlist()->get_gate_by_id(g_0->get_id());
            if (!gates_are_equal(g_0, g_1))
                return false;
            if (!sm1->contains_gate(g_1))
                return false;
        }

        if (sm0->get_nets().size() != sm1->get_nets().size())
            return false;
        for (auto n_0 : sm0->get_nets())
        {
            std::shared_ptr<net> n_1 = sm1->get_netlist()->get_net_by_id(n_0->get_id());
            if (!nets_are_equal(n_0, n_1))
                return false;
            if (!sm1->contains_net(n_1))
                return false;
        }

        return true;
    }

    // Returns true if two netlists have the same content
    bool netlists_are_equal(std::shared_ptr<netlist> nl_0, std::shared_ptr<netlist> nl_1)
    {
        if (nl_0 == nullptr || nl_1 == nullptr)
        {
            if (nl_0 == nl_1)
                return true;
            else
                return false;
        }
        if (nl_0->get_id() != nl_1->get_id())
            return false;
        if (nl_0->get_gate_library()->get_name() != nl_1->get_gate_library()->get_name())
            return false;

        // Check if gates and nets are the same
        if (nl_0->get_gates().size() != nl_1->get_gates().size())
            return false;
        for (auto g_0 : nl_0->get_gates())
        {
            if (!gates_are_equal(g_0, nl_1->get_gate_by_id(g_0->get_id())))
                return false;
        }

        if (nl_0->get_nets().size() != nl_1->get_nets().size())
            return false;
        for (auto n_0 : nl_0->get_nets())
        {
            if (!nets_are_equal(n_0, nl_1->get_net_by_id(n_0->get_id())))
                return false;
        }

        // Check if global gates are the same
        if (nl_0->get_global_gnd_gates().size() != nl_1->get_global_gnd_gates().size())
            return false;
        for (auto gl_gnd_0 : nl_0->get_global_gnd_gates())
        {
            if (!nl_1->is_global_gnd_gate(nl_1->get_gate_by_id(gl_gnd_0->get_id())))
                return false;
        }

        if (nl_0->get_global_vcc_gates().size() != nl_1->get_global_vcc_gates().size())
            return false;
        for (auto gl_vcc_0 : nl_0->get_global_vcc_gates())
        {
            if (!nl_1->is_global_vcc_gate(nl_1->get_gate_by_id(gl_vcc_0->get_id())))
                return false;
        }

        // Check if global nets are the same
        if (nl_0->get_global_input_nets().size() != nl_1->get_global_input_nets().size())
            return false;
        for (auto gl_in_net : nl_0->get_global_input_nets())
        {
            if (!nl_1->is_global_input_net(nl_1->get_net_by_id(gl_in_net->get_id())))
                return false;
        }

        if (nl_0->get_global_output_nets().size() != nl_1->get_global_output_nets().size())
            return false;
        for (auto gl_out_net : nl_0->get_global_output_nets())
        {
            if (!nl_1->is_global_output_net(nl_1->get_net_by_id(gl_out_net->get_id())))
                return false;
        }

        if (nl_0->get_global_inout_nets().size() != nl_1->get_global_inout_nets().size())
            return false;
        for (auto gl_inout_net : nl_0->get_global_inout_nets())
        {
            if (!nl_1->is_global_inout_net(nl_1->get_net_by_id(gl_inout_net->get_id())))
                return false;
        }

        //        // Check if the modules are the same
        //        if(nl_0->get_module_manager()->get_num_of_modules() != nl_1->get_module_manager()->get_num_of_modules())
        //            return false;
        //        std::map<u32, std::shared_ptr<module>> sub_mods_1 = nl_1->get_module_manager()->get_modules();
        //        for(auto sm_0 : nl_0->get_module_manager()->get_modules()){
        //            if(!modules_are_equal(sm_0.second, sub_mods_1[sm_0.second->get_id()]))
        //                return false;
        //        }
        //
        //        // Check if the registered module types are the same
        //        if(nl_0->get_module_manager()->get_module_types() != nl_1->get_module_manager()->get_module_types())
        //            return false;

        return true;
    }
};

/**
 * Testing the serialization and a followed deserialization of the example
 * netlist.
 *
 * Functions: serialize_netlist, deserialize_netlist
 */
TEST_F(netlist_serializer_test, check_serialize_and_deserialize){TEST_START{// Serialize and deserialize the example netlist (with some additions) and compare the result with the original netlist
                                                                            std::shared_ptr<netlist> nl = create_example_netlist(0);
/*
            // Mark some gates as global gates TODO: Not stored?
            nl->add_global_gnd_gate(nl->get_gate_by_id(1));
            nl->add_global_vcc_gate(nl->get_gate_by_id(2));

            // Mark some nets as global nets TODO: Not stored?
            nl->add_global_input_net(nl->get_net_by_id(13));
            nl->add_global_output_net(nl->get_net_by_id(30));
            nl->add_global_inout_net(nl->get_net_by_id(20));

            // Register module types in the module manager TODO: Not stored?
            nl->get_module_manager()->register_module_type("test_type");
            nl->get_module_manager()->register_module_type("test_type_2");
*/
// TODO: decorator?

// Add a module
std::shared_ptr<module> test_sm = nl->create_module(2, "test_type", nl->get_top_module());
            test_sm->assign_gate(nl->get_gate_by_id(1));
            test_sm->assign_gate(nl->get_gate_by_id(2));
            test_sm->assign_net(nl->get_net_by_id(13));
//nl->get_module_manager()->add_module(test_sm);

// Store some data in a gate, net and module
nl->get_gate_by_id(1)->set_data("category_0", "key_0", "data_type", "test_value");
nl->get_gate_by_id(1)->set_data("category_1", "key_1", "data_type", "test_value_1");
nl->get_gate_by_id(1)->set_data("category_1", "key_0", "data_type", "test_value_2");
nl->get_net_by_id(13)->set_data("category", "key_2", "data_type", "test_value");
//nl->get_module_manager()->get_module_by_id(0)->set_data("category", "key_3", "data_type", "test_value");

// Serialize and deserialize the netlist now
test_def::capture_stdout();
bool suc                        = netlist_serializer::serialize_to_file(nl, test_hal_file_path);
std::shared_ptr<netlist> des_nl = netlist_serializer::deserialize_from_file(test_hal_file_path);
test_def::get_captured_stdout();

EXPECT_TRUE(suc);

// Check if the original netlist and the deserialized one are equal

// -- Check if the id and the gate library is the same
EXPECT_EQ(nl->get_id(), des_nl->get_id());
EXPECT_EQ(nl->get_gate_library()->get_name(), des_nl->get_gate_library()->get_name());

// -- Check if gates and nets are the same
EXPECT_EQ(nl->get_gates().size(), des_nl->get_gates().size());
for (auto g_0 : nl->get_gates())
{
    EXPECT_TRUE(gates_are_equal(g_0, des_nl->get_gate_by_id(g_0->get_id())));
}

EXPECT_EQ(nl->get_nets().size(), des_nl->get_nets().size());

for (auto n_0 : nl->get_nets())
{
    EXPECT_TRUE(nets_are_equal(n_0, des_nl->get_net_by_id(n_0->get_id())));
}

// -- Check if global gates are the same
EXPECT_EQ(nl->get_global_gnd_gates().size(), des_nl->get_global_gnd_gates().size());
for (auto gl_gnd_0 : nl->get_global_gnd_gates())
{
    EXPECT_TRUE(des_nl->is_global_gnd_gate(des_nl->get_gate_by_id(gl_gnd_0->get_id())));
}

EXPECT_EQ(nl->get_global_vcc_gates().size(), des_nl->get_global_vcc_gates().size());
for (auto gl_vcc_0 : nl->get_global_vcc_gates())
{
    EXPECT_TRUE(des_nl->is_global_vcc_gate(des_nl->get_gate_by_id(gl_vcc_0->get_id())));
}

// -- Check if global nets are the same
EXPECT_EQ(nl->get_global_input_nets().size(), des_nl->get_global_input_nets().size());
for (auto gl_in_net : nl->get_global_input_nets())
{
    EXPECT_TRUE(des_nl->is_global_input_net(des_nl->get_net_by_id(gl_in_net->get_id())));
}

EXPECT_EQ(nl->get_global_output_nets().size(), des_nl->get_global_output_nets().size());
for (auto gl_out_net : nl->get_global_output_nets())
{
    EXPECT_TRUE(des_nl->is_global_output_net(des_nl->get_net_by_id(gl_out_net->get_id())));
}

EXPECT_EQ(nl->get_global_inout_nets().size(), des_nl->get_global_inout_nets().size());
for (auto gl_inout_net : nl->get_global_inout_nets())
{
    EXPECT_TRUE(des_nl->is_global_inout_net(des_nl->get_net_by_id(gl_inout_net->get_id())));
}

//            // -- Check if the modules are the same
//            EXPECT_EQ(nl->get_num_of_modules(), des_nl->get_num_of_modules());
//            std::map<u32, std::shared_ptr<module>> sub_mods_1 = des_nl->get_module_manager()->get_modules();
//            for(auto sm_0 : nl->get_module_manager()->get_modules()){
//                EXPECT_TRUE(modules_are_equal(sm_0.second, sub_mods_1[sm_0.second->get_id()]));
//            }
//
//            // -- Check if the registered module types are the same
//            EXPECT_EQ(nl->get_module_manager()->get_module_types(), des_nl->get_module_manager()->get_module_types());
}
{
    // Serialize and deserialize an empty netlist and compare the result with the original netlist
    std::shared_ptr<netlist> nl = create_empty_netlist(0);

    test_def::capture_stdout();
    bool suc                        = netlist_serializer::serialize_to_file(nl, test_hal_file_path);
    std::shared_ptr<netlist> des_nl = netlist_serializer::deserialize_from_file(test_hal_file_path);
    test_def::get_captured_stdout();

    EXPECT_TRUE(suc);
    EXPECT_TRUE(netlists_are_equal(nl, des_nl));
}

TEST_END
}

/**
 * Testing the serialization and deserialization of a netlist with invalid input
 *
 * Functions: serialize_netlist, deserialize_netlist
 */
TEST_F(netlist_serializer_test, check_serialize_and_deserialize_negative)
{
    TEST_START
    /*{ TODO: Failed with SIGSEGV
            // Serialize a netlist which is a nullptr
            bool suc = netlist_serializer::serialize_netlist(nullptr, test_hal_file_path);
            EXPECT_FALSE(suc);
        }*/
    {
        // Serialize a netlist to an invalid path
        NO_COUT_TEST_BLOCK;
        std::shared_ptr<netlist> nl = create_example_netlist(0);
        bool suc                    = netlist_serializer::serialize_to_file(nl, hal::path(""));
        EXPECT_FALSE(suc);
    }
    {
        // Deserialize a netlist from a non existing path
        NO_COUT_TEST_BLOCK;
        std::shared_ptr<netlist> nl     = create_example_netlist(0);
        std::shared_ptr<netlist> des_nl = netlist_serializer::deserialize_from_file(hal::path("/using/this/file/is/let.hal"));
        EXPECT_EQ(des_nl, nullptr);
    }
    TEST_END
}
