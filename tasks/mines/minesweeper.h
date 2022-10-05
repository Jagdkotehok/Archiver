#pragma once

#include <string>
#include <vector>
#include <ctime>
#include <random>

class Minesweeper {
public:
    enum class GameStatus {
        NOT_STARTED,
        IN_PROGRESS,
        VICTORY,
        DEFEAT,
    };

    struct Cell {
        size_t x = 0;
        size_t y = 0;
    };

private:
    size_t width_;
    size_t height_;

    struct CellState {
        bool is_with_mine = false;
        bool is_marked = false;
        bool is_opened = false;
    };

    std::time_t start_time_;
    std::time_t finish_time_;

    size_t opened_cells_;
    size_t cells_with_mine_;

    GameStatus current_game_state_;

    std::vector<std::vector<CellState>> cell_state_;
    std::vector<std::vector<Cell>> cells_;

    std::vector<int> dir_x_ = {-1, -1, -1, 0, 0, 1, 1, 1};
    std::vector<int> dir_y_ = {-1, 0, 1, -1, 1, -1, 0, 1};

    const Cell* GetCellByDir(const Cell* cell, size_t direction) const {
        if (cell->x == 0 && dir_x_[direction] == -1) {
            return nullptr;
        }
        if (cell->x == width_ - 1 && dir_x_[direction] == 1) {
            return nullptr;
        }
        if (cell->y == 0 && dir_y_[direction] == -1) {
            return nullptr;
        }
        if (cell->y == height_ - 1 && dir_y_[direction] == 1) {
            return nullptr;
        }
        return &cells_[cell->x + dir_x_[direction]][cell->y + dir_y_[direction]];
    }

public:
    using RenderedField = std::vector<std::string>;

    Minesweeper(size_t width, size_t height, size_t mines_count) {
        width_ = width;
        height_ = height;
        cells_with_mine_ = mines_count;
        opened_cells_ = 0;
        current_game_state_ = GameStatus::NOT_STARTED;
        cell_state_.assign(width_, std::vector<CellState>(height_));
        cells_.assign(width_, std::vector<Cell>(height_));
        for (size_t i = 0; i < width_; ++i) {
            for (size_t j = 0; j < height_; ++j) {
                cells_[i][j] = {i, j};
            }
        }
        std::mt19937 rnd(time(0));
        for (size_t i = 0; i < mines_count; ++i) {  /// this part works in O(N log(N)) where N is size of a field
            while (true) {
                size_t x = rnd() % width_;
                size_t y = rnd() % height_;
                if (cell_state_[x][y].is_with_mine) {
                    continue;
                } else {
                    cell_state_[x][y].is_with_mine = true;
                    break;
                }
            }
        }
    }
    Minesweeper(size_t width, size_t height, const std::vector<Cell>& cells_with_mines) {
        width_ = width;
        height_ = height;
        cells_with_mine_ = 0;
        opened_cells_ = cells_with_mines.size();
        current_game_state_ = GameStatus::NOT_STARTED;
        cell_state_.assign(width_, std::vector<CellState>(height_));
        cells_.assign(width_, std::vector<Cell>(height_));
        for (size_t i = 0; i < width_; ++i) {
            for (size_t j = 0; j < height_; ++j) {
                cells_[i][j] = {i, j};
            }
        }
        for (const Cell& cell : cells_with_mines) {
            cell_state_[cell.x][cell.y].is_with_mine = true;
        }
    }

    void NewGame(size_t width, size_t height, size_t mines_count) {
        width_ = width;
        height_ = height;
        cells_with_mine_ = mines_count;
        opened_cells_ = 0;
        current_game_state_ = GameStatus::NOT_STARTED;
        cell_state_.assign(width_, std::vector<CellState>(height_));
        cells_.assign(width_, std::vector<Cell>(height_));
        for (size_t i = 0; i < width_; ++i) {
            for (size_t j = 0; j < height_; ++j) {
                cells_[i][j] = {i, j};
            }
        }
        std::mt19937 rnd(time(0));
        for (size_t i = 0; i < mines_count; ++i) {  /// this part works in O(N log(N)) where N is size of a field
            while (true) {
                size_t x = rnd() % width_;
                size_t y = rnd() % height_;
                if (cell_state_[x][y].is_with_mine) {
                    continue;
                } else {
                    cell_state_[x][y].is_with_mine = true;
                    break;
                }
            }
        }
    }
    void NewGame(size_t width, size_t height, const std::vector<Cell>& cells_with_mines) {
        width_ = width;
        height_ = height;
        cells_with_mine_ = 0;
        opened_cells_ = cells_with_mines.size();
        current_game_state_ = GameStatus::NOT_STARTED;
        cell_state_.assign(width_, std::vector<CellState>(height_));
        cells_.assign(width_, std::vector<Cell>(height_));
        for (size_t i = 0; i < width_; ++i) {
            for (size_t j = 0; j < height_; ++j) {
                cells_[i][j] = {i, j};
            }
        }
        for (const Cell& cell : cells_with_mines) {
            cell_state_[cell.x][cell.y].is_with_mine = true;
        }
    }

    void OpenCell(const Cell& cell) {
        if (current_game_state_ == GameStatus::NOT_STARTED) {
            current_game_state_ = GameStatus::IN_PROGRESS;
            start_time_ = std::time(nullptr);
        }
        if (current_game_state_ == GameStatus::DEFEAT ||
            current_game_state_ == GameStatus::VICTORY) {  /// game ended so nothing to do here
            return;
        }
        if (cell_state_[cell.x][cell.y].is_marked) {  /// cell marked so nothing to do here
            return;
        }
        if (cell_state_[cell.x][cell.y].is_with_mine) {  /// clicked mine and lost game
            current_game_state_ = GameStatus::DEFEAT;
            finish_time_ = std::time(nullptr);
            for (size_t i = 0; i < width_; ++i) {
                for (size_t j = 0; j < height_; ++j) {
                    cell_state_[i][j].is_opened = true;
                }
            }
        } else {
            std::vector<const Cell*> cells_to_open = {&cell};
            if (!cell_state_[cell.x][cell.y].is_opened) {
                ++opened_cells_;
            }
            cell_state_[cell.x][cell.y].is_opened = true;
            for (size_t i = 0; i < cells_to_open.size(); ++i) {
                const Cell* cur_cell = cells_to_open[i];
                bool any_neighbour_with_mine = false;
                for (size_t direction = 0; direction < 8; ++direction) {
                    const Cell* neighbour = GetCellByDir(cur_cell, direction);
                    if (neighbour == nullptr) {
                        continue;
                    }
                    if (cell_state_[neighbour->x][neighbour->y].is_with_mine) {
                        any_neighbour_with_mine = true;
                    }
                }
                if (any_neighbour_with_mine) {  /// no more opening from this cell
                    continue;
                } else {
                    for (size_t direction = 0; direction < 8; ++direction) {
                        const Cell* neighbour = GetCellByDir(cur_cell, direction);
                        if (neighbour == nullptr) {
                            continue;
                        }
                        if (!cell_state_[neighbour->x][neighbour->y].is_opened &&
                            !cell_state_[neighbour->x][neighbour->y].is_marked) {
                            ++opened_cells_;
                            cell_state_[neighbour->x][neighbour->y].is_opened = true;
                            cells_to_open.emplace_back(neighbour);
                        }
                    }
                }
            }
            if (opened_cells_ + cells_with_mine_ == height_ * width_) {
                current_game_state_ = GameStatus::VICTORY;
                finish_time_ = std::time(nullptr);
            }
        }
    }
    void MarkCell(const Cell& cell) {
        if (current_game_state_ == GameStatus::NOT_STARTED) {
            current_game_state_ = GameStatus::IN_PROGRESS;
            start_time_ = std::time(nullptr);
        }
        if (current_game_state_ == GameStatus::DEFEAT ||
            current_game_state_ == GameStatus::VICTORY) {  /// game ended so nothing to do here
            return;
        }
        if (cell_state_[cell.x][cell.y].is_opened) {  /// cell is opened so nothing to do here
            return;
        }
        if (cell_state_[cell.x][cell.y].is_marked) {  /// unmarking this cell
            cell_state_[cell.x][cell.y].is_marked = false;
            return;
        }
        cell_state_[cell.x][cell.y].is_marked = true;
    }

    GameStatus GetGameStatus() const {
        return current_game_state_;
    }
    time_t GetGameTime() const {
        if (current_game_state_ == GameStatus::NOT_STARTED) {
            return static_cast<std::time_t>(0);
        } else {
            return finish_time_ - start_time_;
        }
    }

    RenderedField RenderField() const {
        RenderedField rendered_field;
        for (size_t j = 0; j < height_; ++j) {
            std::string cur_layer;
            for (size_t i = 0; i < width_; ++i) {
                if (cell_state_[i][j].is_marked) {
                    cur_layer += '?';
                } else if (!cell_state_[i][j].is_opened) {
                    cur_layer += '-';
                } else if (cell_state_[i][j].is_with_mine) {
                    cur_layer += '*';
                } else {
                    size_t cnt_mined_neighbours = 0;
                    for (size_t direction = 0; direction < 8; ++direction) {
                        const Cell* neighbour = GetCellByDir(&cells_[i][j], direction);
                        if (neighbour != nullptr) {
                            if (cell_state_[neighbour->x][neighbour->y].is_with_mine) {
                                ++cnt_mined_neighbours;
                            }
                        }
                    }
                    if (cnt_mined_neighbours == 0) {
                        cur_layer += '.';
                    } else {
                        cur_layer += static_cast<char>(cnt_mined_neighbours + static_cast<size_t>('0'));
                    }
                }
            }
            rendered_field.emplace_back(cur_layer);
        }
        return rendered_field;
    }
};
