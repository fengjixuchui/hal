#include "gui/graph_widget/contexts/module_context.h"

module_context::module_context(const std::shared_ptr<module> m) : graph_context(QString::fromStdString(m->get_name()), m->get_id())
{

}
