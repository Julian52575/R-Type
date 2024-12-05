// Copyright 2024 julian bottiglione
#include "../src/Sparse_Array.hpp"
#include <cassert>
#include <exception>
#include <optional>

static void add_to_10(sparse_array<int>& sp)
{
    for (int i = 0; i < 9; i++) {
        sp.emplace_at(i, i);
        assert(sp[i] == i);
    }
#ifdef DEBUG
    sp.dump();
#endif
    assert(sp.size() == 9); //Invalid sparse_array size
    for (int i = 0; i < 9; i++) {
        assert(sp[i] == i);
    }
}

static void erase_all(sparse_array<int>& sp)
{
    size_t ogSize = sp.size();

    for (int i = 0; i < 9; i++) {
        sp.erase(i);
        try
        {
            std::optional<int> tmp = sp[i];
            assert(tmp.has_value() == false); //no value on erased value
        }
        catch (std::exception &e)
        {
            ;
        }
    }
#ifdef DEBUG
    sp.dump();
#endif
    assert(sp.size() == ogSize);
}

// Test insertion / deletion and ripple effect
static void middle_op(sparse_array<int>& sp)
{
#ifdef DEBUG
    std::cout << "Middle op:" << std::endl;
#endif
    // Setup
    sp.insert_at(0, 0);
    sp.insert_at(2, 2);
    // Insert
    sp.insert_at(1, 1);
#ifdef DEBUG
    sp.dump();
#endif
    assert(sp[1].has_value() == true);
    assert(sp[1].value() == 1);
    assert(sp[0].has_value() == true);
    assert(sp[0].value() == 0);
    assert(sp[2].has_value() == true);
    assert(sp[2].value() == 2);
    // Erasure
    sp.erase(1);
#ifdef DEBUG
    sp.dump();
#endif
    assert(sp[0].has_value() == true);
    assert(sp[0].value() == 0);
    assert(sp[2].has_value() == true);
    assert(sp[2].value() == 2);
    // Emplace
    sp.emplace_at(1, 1);
#ifdef DEBUG
    sp.dump();
#endif
    assert(sp[1].has_value() == true);
    assert(sp[1].value() == 1);
    assert(sp[0].has_value() == true);
    assert(sp[0].value() == 0);
    assert(sp[2].has_value() == true);
    assert(sp[2].value() == 2);
    // Erasure
    sp.erase(1);
#ifdef DEBUG
    sp.dump();
#endif
    assert(sp[0].has_value() == true);
    assert(sp[0].value() == 0);
    assert(sp[2].has_value() == true);
    assert(sp[2].value() == 2);
}

int main(void)
{
    sparse_array<int> sp;

    add_to_10(sp);
    erase_all(sp);
    middle_op(sp);
    return 0;
}
