/*
--- Day 2: 1202 Program Alarm ---

 On the way to your gravity assist around the Moon, your ship computer
 beeps angrily about a "1202 program alarm". On the radio, an Elf is
 already explaining how to handle the situation: "Don't worry, that's
 perfectly norma--" The ship computer bursts into flames.

You notify the Elves that the computer's magic smoke seems to have
escaped. "That computer ran Intcode programs like the gravity assist
program it was working on; surely there are enough spare parts up
there to build a new Intcode computer!"

An Intcode program is a list of integers separated by commas (like
1,0,0,3,99). To run one, start by looking at the first integer (called
position 0). Here, you will find an opcode - either 1, 2, or 99. The
opcode indicates what to do; for example, 99 means that the program is
finished and should immediately halt. Encountering an unknown opcode
means something went wrong.

Opcode 1 adds together numbers read from two positions and stores the
result in a third position. The three integers immediately after the
opcode tell you these three positions - the first two indicate the
positions from which you should read the input values, and the third
indicates the position at which the output should be stored.

For example, if your Intcode computer encounters 1,10,20,30, it should
read the values at positions 10 and 20, add those values, and then
overwrite the value at position 30 with their sum.

Opcode 2 works exactly like opcode 1, except it multiplies the two
inputs instead of adding them. Again, the three integers after the
opcode indicate where the inputs and outputs are, not their values.

Once you're done processing an opcode, move to the next one by
stepping forward 4 positions.

For example, suppose you have the following program:

1,9,10,3,2,3,11,0,99,30,40,50

For the purposes of illustration, here is the same program split into
multiple lines:

1,9,10,3, 2,3,11,0, 99, 30,40,50

The first four integers, 1,9,10,3, are at positions 0, 1, 2, and
3. Together, they represent the first opcode (1, addition), the
positions of the two inputs (9 and 10), and the position of the output
(3). To handle this opcode, you first need to get the values at the
input positions: position 9 contains 30, and position 10 contains
40. Add these numbers together to get 70. Then, store this value at
the output position; here, the output position (3) is at position 3,
so it overwrites itself. Afterward, the program looks like this:

1,9,10,70, 2,3,11,0, 99, 30,40,50

Step forward 4 positions to reach the next opcode, 2. This opcode
works just like the previous, but it multiplies instead of adding. The
inputs are at positions 3 and 11; these positions contain 70 and 50
respectively. Multiplying these produces 3500; this is stored at
position 0:

3500,9,10,70, 2,3,11,0, 99, 30,40,50

Stepping forward 4 more positions arrives at opcode 99, halting the
program.

Here are the initial and final states of a few more small programs:

    1,0,0,0,99 becomes 2,0,0,0,99 (1 + 1 = 2).  2,3,0,3,99 becomes
    2,3,0,6,99 (3 * 2 = 6).  2,4,4,5,99,0 becomes 2,4,4,5,99,9801 (99
    * 99 = 9801).  1,1,1,4,99,5,6,0,99 becomes 30,1,1,4,2,5,6,0,99.

Once you have a working computer, the first step is to restore the
gravity assist program (your puzzle input) to the "1202 program alarm"
state it had just before the last computer caught fire. To do this,
before running the program, replace position 1 with the value 12 and
replace position 2 with the value 2. What value is left at position 0
after the program halts?  

*/

#include <iostream>
#include <fstream>
#include <memory>
#include <vector>

class int_code_computer {
public:
  using pc_t = unsigned int;
  using cell_t = int;
  using mem_t = std::vector<cell_t>;
  
  int_code_computer(std::vector<int> memory)
    : _program_counter{0}
    , _memory{std::move(memory)} {
  }

  bool next() {
    // if _pc is not at the end, perform pc, and increase pc by 4
    if (!finished()) {
      perform();
      _program_counter += 4;
      return !finished();
    }
    return false;
  }

  cell_t& value(pc_t index) {
    return _memory.at(index);
  }
  
  bool finished() { 
    if (_program_counter < _memory.size()) {
      return cur_instruction().opcode == Opcode::HALT;
    }
    return true;
  }

  friend std::ostream& operator << (std::ostream& out, const int_code_computer& computer);
  
private:
  enum class Opcode : cell_t {
     ADD = 1,
     MULT = 2,
     HALT = 99
  };

  struct instruction_t {
    Opcode opcode;
    cell_t args[3];
  };

  // Command is a decoded instruction
  struct command_t {
    Opcode opcode;
    cell_t& a;
    cell_t& b;
    cell_t& dest;

    command_t(const instruction_t& instruction, mem_t& state)
      :opcode(instruction.opcode)
      ,a{state.at(instruction.args[0])}
      ,b{state.at(instruction.args[1])}
      ,dest{state.at(instruction.args[2])}
    {     
    }
  };

  instruction_t& cur_instruction() {
    if (_program_counter >= 0 &&
	_program_counter <= _memory.size()) {
      auto& target = _memory.at(_program_counter);
      return *(reinterpret_cast<instruction_t*>(&target));
    }
    throw std::runtime_error{"pc does not point to a valid instruction!"}; 
  }

  void perform(void) {
    command_t command{cur_instruction(), _memory};
    switch(command.opcode) {
    case Opcode::ADD:
      command.dest = command.a + command.b;
      break;
    case Opcode::MULT:
      command.dest = command.a * command.b;
      break;
    case Opcode::HALT:
      // Note: command is totally bogus here
      break;
    default:
      throw std::runtime_error{"Bad instruction!"};
    }
  }

  
  
  pc_t _program_counter;
  mem_t _memory;
};

std::ostream& operator << (std::ostream& out, const int_code_computer& computer) {
  const auto& mem = computer._memory;
  if (mem.size() > 0) {
    for (size_t i = 0; i < mem.size() - 1; i++) {
      out << mem[i] << ",";
    }
    out << mem[mem.size() - 1];
  }
  return out;
}

int main(const int argc, const char* argv[]) {

  std::unique_ptr<std::ifstream> file_stream_ptr;
  if (argc >= 2) {
    file_stream_ptr = std::make_unique<std::ifstream>(argv[1]);
    if (!file_stream_ptr->is_open()) {
      std::cerr << "Failed to open " << argv[1] << std::endl;
    }
  }
  std::istream& input = file_stream_ptr? *file_stream_ptr : std::cin;
  if(!input.good()){
    std::cerr << "Input is bad!" << std::endl;
  }

  std::vector<int> instructions;
  while(!input.eof()) {
    int next_value;
    if (input >> next_value) {
      instructions.push_back(next_value);
      char comma;
      input >> comma;
    }
  }
  
  std::cout << "Part 1" << std::endl;
  {
    std::vector<int> instructions_restored = instructions;
    instructions_restored.at(1) = 12;
    instructions_restored.at(2) = 2;
    int_code_computer computer{std::move(instructions_restored)};
    while(!computer.finished()) {
      computer.next();
    }  
    std::cout << "Answer (Part 1): " << computer.value(0) << std::endl;
    std::cout << computer << std::endl;
  }

  std::cout << "Part 2" << std::endl;

  for(int i = 0; i < 100; i++) {
    for(int j = 0; j < 100; j++) {
      std::vector<int> instructions_guess = instructions;
      instructions_guess.at(1) = i;
      instructions_guess.at(2) = j;

      int_code_computer computer{std::move(instructions_guess)};
      while(!computer.finished()) { computer.next(); }
      if (computer.value(0) == 19690720) {
	std::cout << "Answer (Part 2): " << i * 100 + j << std::endl;
	break;
      }
    }
  }
  
  return 0;
}
