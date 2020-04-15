#pragma once
#include "lue/gui/selectable.hpp"
#include <imgui.h>
#include <string>


namespace lue {
namespace gui {

class TreeNode:
    public Selectable
{

public:

                   TreeNode            (std::string const& name,
                                        ImGuiTreeNodeFlags const flags=0);

                   TreeNode            (TreeNode const&)=default;

                   TreeNode            (TreeNode&&)=default;

                   ~TreeNode           ();

    TreeNode&      operator=           (TreeNode const&)=default;

    TreeNode&      operator=           (TreeNode&&)=default;

};

}  // namespace gui
}  // namespace lue
