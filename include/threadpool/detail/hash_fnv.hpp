#pragma once
/*
 * hash_32 - 32 bit Fowler/Noll/Vo FNV-1a hash code
 *
 * @(#) $Revision: 5.1 $
 * @(#) $Id: hash_32a.c,v 5.1 2009/06/30 09:13:32 chongo Exp $
 * @(#) $Source: /usr/local/src/cmd/fnv/RCS/hash_32a.c,v $
 *
 ***
 *
 * Fowler/Noll/Vo hash
 *
 * The basis of this hash algorithm was taken from an idea sent
 * as reviewer comments to the IEEE POSIX P1003.2 committee by:
 *
 *      Phong Vo (http://www.research.att.com/info/kpv/)
 *      Glenn Fowler (http://www.research.att.com/~gsf/)
 *
 * In a subsequent ballot round:
 *
 *      Landon Curt Noll (http://www.isthe.com/chongo/)
 *
 * improved on their algorithm.  Some people tried this hash
 * and found that it worked rather well.  In an EMail message
 * to Landon, they named it the ``Fowler/Noll/Vo'' or FNV hash.
 *
 * FNV hashes are designed to be fast while maintaining a low
 * collision rate. The FNV speed allows one to quickly hash lots
 * of data while maintaining a reasonable collision rate.  See:
 *
 *      http://www.isthe.com/chongo/tech/comp/fnv/index.html
 *
 * for more details as well as other forms of the FNV hash.
 ***
 *
 * To use the recommended 32 bit FNV-1a hash, pass FNV1_32A_INIT as the
 * Fnv32_t hashval argument to fnv_32a_buf() or fnv_32a_str().
 *
 ***
 *
 * Please do not copyright this code.  This code is in the public domain.
 *
 * LANDON CURT NOLL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO
 * EVENT SHALL LANDON CURT NOLL BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 * By:
 *	chongo <Landon Curt Noll> /\oo/\
 *      http://www.isthe.com/chongo/
 *
 * Share and Enjoy!	:-)
 */
#include <string>
#include <vector>
#include <algorithm>
#include <map>

namespace thread_pool
{
	namespace detail
	{
		uint32_t fnv_32a_buf(void* buf, size_t len, uint32_t hval)
		{
			unsigned char* bp = (unsigned char*)buf;	/* start of buffer */
			unsigned char* be = bp + len;		/* beyond end of buffer */

			/*
			 * FNV-1a hash each octet in the buffer
			 */
			while (bp < be) {

				/* xor the bottom with the current octet */
				hval ^= (uint32_t)*bp++;

				/* multiply by the 32 bit FNV magic prime mod 2^32 */
#if defined(NO_FNV_GCC_OPTIMIZATION)
				hval *= FNV_32_PRIME;
#else
				hval += (hval << 1) + (hval << 4) + (hval << 7) + (hval << 8) + (hval << 24);
#endif
			}

			/* return our new hash value */
			return hval;
		}

		class hash_fnv
		{
			inline constexpr static uint32_t fvn_prime = 0x01000193;

			inline constexpr static int virtual_nodes = 20;

		public:
			uint32_t hash(const std::string& buf)
			{
				return fnv_32a_buf(reinterpret_cast<void*>(const_cast<char*>(buf.data())), buf.size(), fvn_prime);
			}

			void put_real(const std::vector<std::string>& vec)
			{
				std::for_each(vec.begin(), vec.end(),
					[&, this](auto v)
					{
						for (int i = 0; i < virtual_nodes; i++)
						{
							std::string virtual_node_name = construct_virtual_name(v, i);

							unsigned int hash_code = hash(virtual_node_name);

							virtual_list_.insert({ hash_code,virtual_node_name });
						}
					});
			}

			std::string get_node(const std::string& node)
			{
				unsigned int hash_node = hash(node);

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
			std::map<unsigned int, std::string> virtual_list_;
		};
	}
}