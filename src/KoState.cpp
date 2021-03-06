/*
    This file is part of Leela Zero.
    Copyright (C) 2017 Gian-Carlo Pascutto

    Leela Zero is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Leela Zero is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Leela Zero.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <string>
#include <algorithm>

#include "config.h"

#include "FastState.h"
#include "FullBoard.h"
#include "KoState.h"

void KoState::init_game(int size, float komi) {
    assert(size <= FastBoard::MAXBOARDSIZE);

    FastState::init_game(size, komi);

    m_ko_hash_history.clear();
    m_hash_history.clear();

    m_ko_hash_history.emplace_back(board.calc_ko_hash());
    m_hash_history.emplace_back(board.calc_hash());
}

bool KoState::superko(void) const {
    auto first = crbegin(m_ko_hash_history);
    auto last = crend(m_ko_hash_history);

    auto res = std::find(++first, last, board.get_ko_hash());

    return (res != last);
}

bool KoState::superko(uint64 newhash) const {
    auto first = crbegin(m_ko_hash_history);
    auto last = crend(m_ko_hash_history);

    auto res = std::find(first, last, newhash);

    return (res != last);
}

void KoState::reset_game() {
    FastState::reset_game();

    m_ko_hash_history.clear();
    m_hash_history.clear();

    m_ko_hash_history.push_back(board.calc_ko_hash());
    m_hash_history.push_back(board.calc_hash());
}

void KoState::play_pass(void) {
    FastState::play_pass();

    m_ko_hash_history.push_back(board.get_ko_hash());
    m_hash_history.push_back(board.get_hash());
}

void KoState::play_move(int vertex) {
    play_move(board.get_to_move(), vertex);
}

void KoState::play_move(int color, int vertex) {
    if (vertex != FastBoard::PASS && vertex != FastBoard::RESIGN) {
        FastState::play_move(color, vertex);

        m_ko_hash_history.push_back(board.get_ko_hash());
        m_hash_history.push_back(board.get_hash());
    } else {
        play_pass();
    }
}
