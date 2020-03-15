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
            {0x78F1E3C78Full, 1, 2, 3},  // row
            {0x1FFFFFull, 7, 14, 21},  // column
            {0x1E3C78ull, 6, 12, 18},  // left diagonal
            {0x3C78Full, 8, 16, 24}  // righ diagonal
        };

        for (int i = 0; i < 4; ++i) {
            uint64_t tokenstmp = tokens & (tokens >> m[i].s2);
            if (tokenstmp & (tokenstmp >> m[i].s1) & m[i].mask) {
                size = 0;
                return;
            }
        }

        move_mask <<= 7;
        if (move_mask >= (1ull << 42)) {
            if (size == 1) {
                size = 0;
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
