// THIS IS WHAT THE RANDOM SOURCE FILE SHOULD LOOK LIKE IN WALLNUT
//CHANGE std::mt19937 Random::s_RandomEngine; TO thread_local std::mt19937 Random::s_RandomEngine;
#include "Random.h"

namespace Walnut {

	thread_local std::mt19937 Random::s_RandomEngine;
	std::uniform_int_distribution<std::mt19937::result_type> Random::s_Distribution;

}