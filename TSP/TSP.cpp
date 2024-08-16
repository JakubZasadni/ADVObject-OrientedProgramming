#include "TSP.hpp"
#include <algorithm>
#include <stack>
#include <optional>

std::ostream &operator<<(std::ostream &os, const CostMatrix &cm) {
    for (std::size_t r = 0; r < cm.size(); ++r) {
        for (std::size_t c = 0; c < cm.size(); ++c) {
            const auto &elem = cm[r][c];
            os << (is_inf(elem) ? "INF" : std::to_string(elem)) << " ";
        }
        os << "\n";
    }
    os << std::endl;

    return os;
}

/* PART 1 */

/**
 * Create path from unsorted path and last 2x2 cost matrix.
 * @return The vector of consecutive vertex.
 */
path_t StageState::get_path() {
    NewVertex vertex = choose_new_vertex();
    append_to_path(vertex.coordinates);
    update_cost_matrix(vertex.coordinates);
    for (std::size_t i = 0; i < matrix_.size(); i++) {
        for (std::size_t j = 0; j < matrix_.size(); j++) {
            if (matrix_[i][j] != INF) {
                append_to_path(vertex_t(i, j));
            }
        }
    }
    path_t sorted_path = {unsorted_path_[0].col};
    unsorted_path_[0].row = INF;
    while(sorted_path.size() != unsorted_path_.size()) {
        for (std::size_t i = 0; i < unsorted_path_.size(); i++) {
            if (unsorted_path_[i].row == sorted_path.back()) {
                sorted_path.push_back(unsorted_path_[i].col);
            }
        }
    }

    return sorted_path;
}

/**
 * Get minimum values from each row and returns them.
 * @return Vector of minimum values in row.
 */
std::vector<cost_t> CostMatrix::get_min_values_in_rows() const {
    std::vector<cost_t> min_value_vector = {};
    for (auto &row: matrix_) {
        cost_t min_value = INF;
        for (auto elem: row) {
            if (elem < min_value) {
                min_value = elem;
            }
        }
        if (min_value == INF) {
            min_value = 0;
        }
        min_value_vector.push_back(min_value);
    }
    return min_value_vector;
}

/**
 * Reduce rows so that in each row at least one zero value is present.
 * @return Sum of values reduced in rows.
 */
cost_t CostMatrix::reduce_rows() {
    cost_t sum = 0;
    std::vector<cost_t> min_vector = get_min_values_in_rows();
    for (std::size_t i = 0; i < matrix_.size(); i++) {
        for (std::size_t j = 0; j < matrix_[0].size(); j++) {
            if (matrix_[i][j] != INF) {
                matrix_[i][j] -= min_vector[i];
            }
        }
        sum += min_vector[i];
    }
    return sum;
}

/**
 * Get minimum values from each column and returns them.
 * @return Vector of minimum values in columns.
 */
std::vector<cost_t> CostMatrix::get_min_values_in_cols() const {
    std::vector<cost_t> min_value_vector = {};
    for (std::size_t j = 0; j < matrix_[0].size(); j++) {
        cost_t min_value = INF;
        for (std::size_t i = 0; i < matrix_.size(); i++) {
            if (matrix_[i][j] < min_value) {
                min_value = matrix_[i][j];
            }
        }
        if (min_value == INF) {
            min_value = 0;
        }
        min_value_vector.push_back(min_value);
    }
    return min_value_vector;
}

/**
 * Reduces rows so that in each column at least one zero value is present.
 * @return Sum of values reduced in columns.
 */
cost_t CostMatrix::reduce_cols() {
    cost_t sum = 0;
    std::vector<cost_t> min_vector = get_min_values_in_cols();
    for (std::size_t j = 0; j < matrix_[0].size(); j++) {
        for (std::size_t i = 0; i < matrix_.size(); i++) {
            if (matrix_[i][j] != INF) {
                matrix_[i][j] -= min_vector[j];
            }
        }
        sum += min_vector[j];
    }
    return sum;
}

/**
 * Get the cost of not visiting the vertex_t (@see: get_new_vertex())
 * @param row
 * @param col
 * @return The sum of minimal values in row and col, excluding the intersection value.
 */
cost_t CostMatrix::get_vertex_cost(std::size_t row, std::size_t col) const {
    cost_t min_value_r = INF;
    cost_t min_value_c = INF;
    for (std::size_t j = 0; j < matrix_[0].size(); j++) {
        if (j != col) {
            if (matrix_[row][j] < min_value_r) {
                min_value_r = matrix_[row][j];
            }
        }
    }
    for (std::size_t i = 0; i < matrix_.size(); i++) {
        if (i != row) {
            if (matrix_[i][col] < min_value_c) {
                min_value_c = matrix_[i][col];
            }
        }
    }
    return min_value_c + min_value_r;
}

/* PART 2 */

/**
 * Choose next vertex to visit:
 * - Look for vertex_t (pair row and column) with value 0 in the current cost matrix.
 * - Get the vertex_t cost (calls get_vertex_cost()).
 * - Choose the vertex_t with maximum cost and returns it.
 * @param cm
 * @return The coordinates of the next vertex.
 */
NewVertex StageState::choose_new_vertex() {
    cost_t max_value = 0;
    vertex_t vertex;
    for (std::size_t i = 0; i < matrix_.size(); i++) {
        for (std::size_t j = 0; j < matrix_[0].size(); j++) {
            if (matrix_[i][j] == 0) {
                cost_t cost = matrix_.get_vertex_cost(i, j);
                if (cost > max_value) {
                    max_value = cost;
                    vertex.row = i;
                    vertex.col = j;
                }
            }
        }
    }
    return NewVertex(vertex, max_value);
}

/**
 * Update the cost matrix with the new vertex.
 * @param new_vertex
 */
void StageState::update_cost_matrix(vertex_t new_vertex) {
    for (std::size_t i = 0; i < matrix_.size(); i++) {
        for (std::size_t j = 0; j < matrix_[0].size(); j++) {
            if (i == new_vertex.row or j == new_vertex.col) {
                matrix_[i][j] = INF;
            }
        }
    }
    matrix_[new_vertex.col][new_vertex.row] = INF;

}

/**
 * Reduce the cost matrix.
 * @return The sum of reduced values.
 */
cost_t StageState::reduce_cost_matrix() {
    cost_t sum_r = matrix_.reduce_rows();
    cost_t sum_c = matrix_.reduce_cols();
    return sum_c + sum_r;
}

/**
 * Given the optimal path, return the optimal cost.
 * @param optimal_path
 * @param m
 * @return Cost of the path.
 */
cost_t get_optimal_cost(const path_t &optimal_path, const cost_matrix_t &m) {
    cost_t cost = 0;

    for (std::size_t idx = 1; idx < optimal_path.size(); ++idx) {
        cost += m[optimal_path[idx - 1]][optimal_path[idx]];
    }

    // Add the cost of returning from the last city to the initial one.
    cost += m[optimal_path.back()][optimal_path[0]];

    return cost;
}

/**
 * Create the right branch matrix with the chosen vertex forbidden and the new lower bound.
 * @param m
 * @param v
 * @param lb
 * @return New branch.
 */
StageState create_right_branch_matrix(cost_matrix_t m, vertex_t v, cost_t lb) {
    CostMatrix cm(m);
    cm[v.row][v.col] = INF;
    return StageState(cm, {}, lb);
}

/**
 * Retain only optimal ones (from all possible ones).
 * @param solutions
 * @return Vector of optimal solutions.
 */
tsp_solutions_t filter_solutions(tsp_solutions_t solutions) {
    auto min_element_it = std::min_element(solutions.begin(), solutions.end(),[](const tsp_solution_t &a, const tsp_solution_t &b) {
        return a.lower_bound < b.lower_bound;
    });

    cost_t optimal_cost = (min_element_it != solutions.end()) ? min_element_it->lower_bound : INF;


    tsp_solutions_t optimal_solutions;
    std::copy_if(solutions.begin(), solutions.end(),
                 std::back_inserter(optimal_solutions),
                 [&optimal_cost](const tsp_solution_t &s) { return s.lower_bound == optimal_cost; }
    );

    return optimal_solutions;
}

/**
 * Solve the TSP.
 * @param cm The cost matrix.
 * @return A list of optimal solutions.
 */
tsp_solutions_t solve_tsp(const cost_matrix_t &cm) {

    StageState left_branch(cm);

    // The branch & bound tree.
    std::stack<StageState> tree_lifo;

    // The number of levels determines the number of steps before obtaining
    // a 2x2 matrix.
    std::size_t n_levels = cm.size() - 2;

    tree_lifo.push(left_branch);   // Use the first cost matrix as the root.

    cost_t best_lb = INF;
    tsp_solutions_t solutions;

    while (!tree_lifo.empty()) {

        left_branch = tree_lifo.top();
        tree_lifo.pop();

        while (left_branch.get_level() != n_levels && left_branch.get_lower_bound() <= best_lb) {
            // Repeat until a 2x2 matrix is obtained or the lower bound is too high...

            if (left_branch.get_level() == 0) {
                left_branch.reset_lower_bound();
            }

            // 1. Reduce the matrix in rows and columns.
            cost_t new_cost = 0; // @TODO (KROK 1)
            new_cost = left_branch.reduce_cost_matrix();


            // 2. Update the lower bound and check the break condition.
            left_branch.update_lower_bound(new_cost);
            if (left_branch.get_lower_bound() > best_lb) {
                break;
            }

            // 3. Get new vertex and the cost of not choosing it.
            NewVertex new_vertex = NewVertex(); // @TODO (KROK 2)
            new_vertex = left_branch.choose_new_vertex();

            // 4. @TODO Update the path - use append_to_path method.
            left_branch.append_to_path(new_vertex.coordinates);

            // 5. @TODO (KROK 3) Update the cost matrix of the left branch.
            left_branch.update_cost_matrix(new_vertex.coordinates);

            // 6. Update the right branch and push it to the LIFO.
            cost_t new_lower_bound = left_branch.get_lower_bound() + new_vertex.cost;
            tree_lifo.push(create_right_branch_matrix(cm, new_vertex.coordinates,
                                                      new_lower_bound));
        }

        if (left_branch.get_lower_bound() <= best_lb) {
            // If the new solution is at least as good as the previous one,
            // save its lower bound and its path.
            best_lb = left_branch.get_lower_bound();
            path_t new_path = left_branch.get_path();
            solutions.push_back({get_optimal_cost(new_path, cm), new_path});
        }
    }

    return filter_solutions(solutions); // Filter solutions to find only optimal ones.
}
