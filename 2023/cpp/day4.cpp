#include <iostream>
#include <filesystem>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <fstream>

using std::string;
using std::istringstream;
using std::filesystem::path;
using std::vector;

struct Card {
  vector<int> winners;
  vector<int> on_card;
};

Card parse_line(const string& line) {
  size_t colon_pos {line.find(':')};
  if (colon_pos == string::npos) {
    throw std::runtime_error("No colon found in line: " + line);
  }

  string after_colon = line.substr(colon_pos + 1);

  istringstream iss(after_colon);
  string word;

  vector<int> winners;
  vector<int> on_card;
  bool past_pipe {false};
  
  while (iss >> word) {
    if (word == "|") {
      past_pipe = true;
    } else if (past_pipe) {
      on_card.push_back(std::stoi(word));
    } else {
      winners.push_back(std::stoi(word));
    }
  }

  std::sort(winners.begin(), winners.end());
  std::sort(on_card.begin(), on_card.end());
  
  return Card{winners, on_card};
}

long int points(const Card& card) {
  int num_winners {0};
  for (auto& winner : card.winners) {
    if (std::find(card.on_card.begin(), card.on_card.end(), winner) != card.on_card.end()) {
      num_winners++;
    }
  }
  // score is 2^num_winners
  if (num_winners <= 0) {
    return 0;
  }
  return 1 << (num_winners - 1);
}

long int points_from_file(const path& p) {
  std::ifstream ifs(p);
  string line;
  long int total {0};
  while (std::getline(ifs, line)) {
    Card card = parse_line(line);
    Card& card_ref = card;
    long int card_points = points(card_ref);
    total += card_points;
  }
  return total;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " <file>\n";
    return 1;
  }

  path p = argv[1];

  if (!std::filesystem::exists(p)) {
    std::cout << "File " << p << " does not exist.\n";
    return 1;
  }

  long int points = points_from_file(p);

  std::cout << "Total points: " << points << "\n";
}

