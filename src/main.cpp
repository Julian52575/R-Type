#include "ComponentRegistry.hpp"

int main(void)
{
    Rengine::ComponentRegistry a;
    Rengine::SparseArray<int>& s = a.registerComponent<int>();
    Rengine::SparseArray<int>& s2 = a.getComponents<int>();

    return 84;
}
