#ifndef PATTERN_POLAR_REP_HPP_
#define PATTERN_POLAR_REP_HPP_

#include <stdexcept>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <typeinfo>

#include "Pattern_polar_i.hpp"

#include "Pattern_polar_r0.hpp"
#include "Pattern_polar_r1.hpp"

namespace aff3ct
{
namespace tools
{
class Pattern_polar_rep : public Pattern_polar_i
{
protected:
	Pattern_polar_rep(const int &N, const Binary_node<Pattern_polar_i>* node,
	                  const int min_level = 1, const int max_level = -1)
	: Pattern_polar_i(N, node, min_level, max_level)
	{
		if (min_level < 1)
		{
			std::stringstream message;
			message << "'min_level' has to be equal or greater than 1 ('min_level' = " << min_level << ").";
			throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}
	}

public:
	Pattern_polar_rep(const int min_level = 1, const int max_level = -1)
	: Pattern_polar_i(min_level, max_level)
	{
		if (min_level < 1)
		{
			std::stringstream message;
			message << "'min_level' has to be equal or greater than 1 ('min_level' = " << min_level << ").";
			throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}
	}

	virtual Pattern_polar_i* alloc(const int &N, const Binary_node<Pattern_polar_i>* node) const
	{
		return new Pattern_polar_rep(N, node, min_level, max_level);
	}

	virtual ~Pattern_polar_rep() = default;

	virtual polar_node_t type()       const { return polar_node_t::REP; }
	virtual std::string  name()       const { return "Rep";             }
	virtual std::string  short_name() const { return "re";              }
	virtual std::string  fill_color() const { return "#8F463F";         }
	virtual std::string  font_color() const { return "#FFFFFF";         }

	virtual std::string f() const { return "";    }
	virtual std::string g() const { return "";    }
	virtual std::string h() const { return "rep"; }

	static bool recursive_check(const int reverse_graph_depth, const Binary_node<Pattern_polar_i>* node_curr)
	{
		if (!node_curr->is_leaf())
		{
			const auto pattern_left  = node_curr->get_left ()->get_contents();
			const auto pattern_right = node_curr->get_right()->get_contents();

			if (pattern_left->type() == polar_node_t::RATE_0 ||
				Pattern_polar_r0::recursive_check(reverse_graph_depth -1, node_curr->get_left()))
			{
				if (pattern_right->type() == polar_node_t::REP)
					return true;
				else if (pattern_right->type() == polar_node_t::RATE_1 && reverse_graph_depth == 1)
					return true;
				else
					return Pattern_polar_rep::recursive_check(reverse_graph_depth -1, node_curr->get_right());
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	virtual int _match(const int &reverse_graph_depth, const Binary_node<Pattern_polar_i>* node_curr) const
	{
		return Pattern_polar_rep::recursive_check(reverse_graph_depth, node_curr) ? 50 : 0;
	}

	virtual bool is_terminal() const { return true; }
};
}
}

#endif /* PATTERN_POLAR_REP_HPP_ */
