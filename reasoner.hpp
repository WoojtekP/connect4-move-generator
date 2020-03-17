#ifndef REASONER
#define REASONER

#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <cstdint>
#include <string>
#include <vector>

namespace reasoner {
    constexpr int NUMBER_OF_PLAYERS = 3;
    constexpr int MONOTONIC_CLASSES = 1;

    class resettable_bitarray_stack {};

    typedef uint64_t move_representation;
    struct move {
        move_representation mr;
        move(const move_representation& mv) : mr(mv) {};
        move(void) = default;
        bool operator==(const move& rhs) const {
            return mr == rhs.mr;
        }
    };
    enum player {
        KEEPER = 0,
        RED = 1,
        YELLOW = 2
    };
    struct line_matcher {
        uint8_t s1;
        uint8_t s2;
    };
    class game_state {
        public:
            int get_current_player(void) const;
            int get_player_score(int player_id) const;
            void apply_move(const move&);
            void apply_move(const uint&);
            std::vector<move> get_all_moves(resettable_bitarray_stack& cache);
            void get_all_moves(resettable_bitarray_stack&, std::vector<move>& moves);
            bool apply_any_move(resettable_bitarray_stack&);
            int get_monotonicity_class(void);
            bool is_legal(const move& m) const;
            uint get_moves_count() const;
        private:
            uint64_t red = 0;
            uint64_t yellow = 0;
            uint size = 7;
            int current_cell = 1;
            player current_player = RED;
            int current_state = 0;
            int variables[2] = {0, 0};
            std::vector<move> all_moves {
                1ull,
                1ull << 1,
                1ull << 2,
                1ull << 3,
                1ull << 4,
                1ull << 5,
                1ull << 6,
            };
    };
}

#endif
