#include "reasoner.hpp"

namespace reasoner {
    int game_state::get_current_player(void) const {
        return current_player;
    }

    int game_state::get_player_score(int player_id) const {
        return variables[player_id-1];
    }

    void game_state::apply_move(const uint& move_id) {
        auto move_mask = all_moves[move_id].mr;
        uint64_t tokens = (current_player == YELLOW) ? yellow : red;

        tokens |= move_mask;

        static constexpr line_matcher m[] = {
            {1, 2},  // row
            {8, 16},  // column
            {7, 14},  // left diagonal
            {9, 18}  // righ diagonal
        };

        uint64_t result = 0;
        for (int i = 0; i < 4; ++i) {
            uint64_t tokenstmp = tokens & (tokens >> m[i].s2);
            result |= tokenstmp & (tokenstmp >> m[i].s1);
            // if (tokenstmp & (tokenstmp >> m[i].s1)) {
            //     size = 0;
            //     variables[current_player - 1] = 100;
            //     variables[current_player % 2] = 0;
            //     return;
            // }
        }
        if (result) {
            size = 0;
            // variables[current_player - 1] = 100;
            // variables[current_player % 2] = 0;
            return;
        }

        move_mask <<= 8;
        if (move_mask > (1ull << 47)) {
            if (size == 1) {
                size = 0;
                // variables[0] = variables[1] = 50;
                return;
            }
            all_moves[move_id].mr = all_moves.back().mr;
            size--;
        }
        else {
            all_moves[move_id].mr = move_mask;
        }

        if (current_player == RED) {
            red = tokens;
            current_player = YELLOW;
        }
        else {
            yellow = tokens;
            current_player = RED;
        }
    }

    std::vector<move> game_state::get_all_moves(resettable_bitarray_stack& cache) {
        std::vector<move> result;
        result.reserve(100);
        get_all_moves(cache, result);
        return result;
    }

    void game_state::get_all_moves(resettable_bitarray_stack&, std::vector<move>&) {
        return;
    }

    bool game_state::apply_any_move(resettable_bitarray_stack&) {
        return false;
    }

    int game_state::get_monotonicity_class(void) {
        return -1;
    }

    bool game_state::is_legal([[maybe_unused]] const move& m) const {
        return false;
    }

    uint game_state::get_moves_count() const {
        return size;
    }

}
