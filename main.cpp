#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <numeric>
#include <omp.h>

using namespace std;

enum class Direction {
    Left = -1,
    Right = 1
};

struct Ant {
    int id;
    int x;
    Direction dir;
    bool is_active;

    Direction switch_dir()
    {
        return dir == Direction::Left ? Direction::Right : Direction::Left;
    }
    bool goes_to_edge(int total)
    {
        return (id == 0 && dir == Direction::Left) || (id == total - 1 && dir == Direction::Right);
    }
};

int operator+(const int lhs, const Direction rhs)
{
    return lhs + static_cast<int>(rhs);
}
int operator+(const Direction lhs, const int rhs)
{
    return rhs + lhs;
}

void print_state(const vector<Ant>& state, int stick_length) 
{
    string stick_vis(stick_length + 1, '-');

    for (const auto& ant : state) 
    {
        if (ant.is_active && ant.x >= 0 && ant.x <= stick_length) 
        {
            if (stick_vis[ant.x] != '-') stick_vis[ant.x] = 'X'; 
            else stick_vis[ant.x] = (ant.dir == Direction::Left) ? '<' : '>';

        }
    }

    cout << stick_vis << '\n';
}

vector<Ant> initialize_ants(int num_ants, int stick_length) 
{
    vector<Ant> initial_state(num_ants);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dir_dist(0, 1);

    vector<int> all_positions(stick_length + 1);
    iota(all_positions.begin(), all_positions.end(), 0);

    vector<int> chosen_positions;
    sample(all_positions.begin(), all_positions.end(), 
           back_inserter(chosen_positions), num_ants, gen);

    sort(chosen_positions.begin(), chosen_positions.end());

    for (int i = 0; i < num_ants; ++i) {
        initial_state[i].id = i;
        initial_state[i].x = chosen_positions[i];
        initial_state[i].dir = (dir_dist(gen) == 0) ? Direction::Left : Direction::Right;
        initial_state[i].is_active = true;
    }

    return initial_state;
}

int main(int argc, char* argv[]) 
{
    if (argc < 3) 
    {
        cerr << "Usage: " << argv[0] << " <stick_length> <num_of_ants>\n";
        return 1;
    }

    int stick_length = stoi(argv[1]);
    int num_ants = stoi(argv[2]);

    if (num_ants > stick_length + 1) 
    {
        cerr << "Error: Number of ants cannot exceed stick length + 1.\n";
        return 1;
    }

    int ants_on_stick = num_ants;
    int time_step = 0;

    vector<Ant> state_now = initialize_ants(num_ants, stick_length);
    vector<Ant> state_next(num_ants);

    cout << "Step 0:\n";
    print_state(state_now, stick_length);

    while (ants_on_stick > 0) 
    {
        
        #pragma omp parallel for
        for (int i = 0; i < num_ants; ++i) 
        {
            if (!state_now[i].is_active) 
            {
                state_next[i] = state_now[i];
                continue;
            }

            state_next[i] = state_now[i];

            if( !state_now[i].goes_to_edge(num_ants)
                && state_now[i + state_now[i].dir].is_active
                && state_now[i].x + state_now[i].dir == state_now[i + state_now[i].dir].x
                && state_now[i].dir != state_now[i + state_now[i].dir].dir)
            {
                state_next[i].x = state_now[i].x;
                state_next[i].dir = state_now[i].switch_dir();
            }
            else
            {
                state_next[i].x = state_now[i].x + state_now[i].dir;
                state_next[i].dir = state_now[i].dir;
            }
        }

        #pragma omp parallel for
        for (int i = 0; i < num_ants; ++i) 
        {
            if (!state_next[i].is_active) continue;

            if( !state_next[i].goes_to_edge(num_ants)
                && state_next[i + state_next[i].dir].is_active
                && state_next[i].x == state_next[i + state_next[i].dir].x) state_next[i].dir = state_next[i].switch_dir();

        }

        int current_active = 0;
        #pragma omp parallel for reduction(+:current_active)
        for (int i = 0; i < num_ants; ++i) 
        {
            if (state_next[i].is_active) 
            {
                if (state_next[i].x < 0 || state_next[i].x > stick_length) 
                {
                    state_next[i].is_active = false;
                } else 
                {
                    current_active++;
                }
            }
        }
        ants_on_stick = current_active;

        swap(state_now, state_next);
        time_step++;

        cout << "Step " << time_step << ":\n";
        print_state(state_now, stick_length);
    }

    cout << "Simulation finished in " << time_step << " steps.\n";

    return 0;
}