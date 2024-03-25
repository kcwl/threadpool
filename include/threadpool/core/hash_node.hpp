#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <map>

namespace thread_pool
{
	namespace detail
	{
		class hash_node
		{
			inline constexpr static uint32_t fvn_prime = 0x01000193;

			inline constexpr static int virtual_nodes = 20;

		public:
			void put_real(const std::vector<std::string>& vec)
			{
				std::for_each(vec.begin(), vec.end(),
					[&, this](auto v)
					{
						for (int i = 0; i < virtual_nodes; i++)
						{
							std::string virtual_node_name = construct_virtual_name(v, i);

							auto hash_code = std::hash<std::string>{}(virtual_node_name);

							virtual_list_.insert({ hash_code,virtual_node_name });
						}
					});
			}

			std::string get_node(const std::string& node)
			{
				auto hash_node = std::hash<std::string>{}(node);

				auto iter = std::find_if(virtual_list_.begin(), virtual_list_.end(),
					[&](auto v)
					{
						return v.first >= hash_node;
					});

				if (iter == virtual_list_.end())
					return {};

				return iter->second;
			}

		private:
			std::string construct_virtual_name(const std::string& name, int num)
			{
				return name + "&VN" + std::to_string(num);
			}

			std::string deconstruct_real_name(const std::string& name)
			{
				auto pos = name.find_first_of('&');

				if (pos == std::string::npos)
					return name;

				return name.substr(0, pos);
			}

		private:
			std::map<std::size_t, std::string> virtual_list_;
		};
	}
}