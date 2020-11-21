#ifndef HASH_MAP_HPP
#define HASH_MAP_HPP


#include <array>
#include <memory>

/**
 * Implementation a hash table of constant size.
 *
 * @param T Type of the entry value
 * @param K Type of the entry key
 * @param Size Constant size of the hash table
 * @param Hash Struct with overloaded operator() as with hash function
*/
template <class T, class K, size_t Size, class Hash = std::hash<K>>
class HashMap {
	using Entry = std::pair<const K, T>;
	using EntryUPtr = std::unique_ptr<Entry>;
	
	std::array<EntryUPtr, Size> m_table;
	Hash m_hash;

public:
	/**
	 * Default constructor for HastMap.
	 * Time: O(1)
	 * Space: O(1)
	 * 
	 * @return HashMap
	 */
	HashMap() : m_table{ std::array<EntryUPtr, Size>{} }, m_hash{ Hash{} }{}

	/**
	 * Insert a new element in the hash table if no element already has the key.
	 * Time: O(1)
	 * Space: O(1)
	 *
	 * @param  key Key to insert
	 * @param  value Value to map to the key
	 * @return Pointer to the newly inserted pair OR the previously mapped element
	 */
	const std::pair<bool, Entry*> insert(const K& key, const T& value);
	
	
	/**
	 * Finds an element on the hash table.
	 * Time: O(1)
	 * Space: O(1)
	 *
	 * @param  key Key to insert
	 * @return Pointer to the found entry or nullptr if not found
	 */
	 Entry* find(const K& key);

	 /**
	 * Erases an entry with a given key.
	 * Time: O(1)
	 * Space: O(1)
	 *
	 * @param  key Key of the entry to erase
	 */
	 void erase(const K& key);

private:
	/**
	 * Generates a container index mapped to the key.
	 * Time: O(1)
	 * Space: O(1)
	 *
	 * @param  key Key of the entry to hash
	 * @return Index of the table mapped to the key
	 */
	size_t hash(const K& key) const;

	/**
	 * Finds the node element of the given key. 
	 * Private helper for other functions
	 * Time: O(1)
	 * Space: O(1)
	 *
	 * @param  key Key of the entry to hash
	 * @param [out] node found found at the position
	 * @return Index of the table mapped to the key
	 */
	size_t findNode(const K& key, EntryUPtr*& node);
	 	
};

template<class T, class K, size_t Size, class Hash>
inline const std::pair<bool, typename HashMap<T, K, Size, Hash>::Entry*> HashMap<T, K, Size, Hash>::insert(const K& key, const T& value){
	EntryUPtr* res{nullptr};
	size_t i{ findNode(key, res) };
	
	// Check if the given position is 
	if (res != nullptr && *res != nullptr) {
		// The key is occupied, return the element
		return { false, m_table[i].get() };
		
	}
	else {
		// Position was empty, insert
		m_table[i] = std::make_unique<Entry>(key, value);
		return { true, m_table[i].get() };
		// TODO: Debug this.
	}
}

template<class T, class K, size_t Size, class Hash>
inline typename HashMap<T, K, Size, Hash>::Entry* HashMap<T, K, Size, Hash>::find(const K& key){
	EntryUPtr* res{ nullptr };
	findNode(key, res);
	return ((res != nullptr && *res != nullptr) ? res->get() : nullptr);
}

template<class T, class K, size_t Size, class Hash>
inline size_t HashMap<T, K, Size, Hash>::hash(const K& key) const{
	return m_hash(key) % Size;
}

template<class T, class K, size_t Size, class Hash>
inline size_t HashMap<T, K, Size, Hash>::findNode(const K& key, EntryUPtr*& node){

	size_t startPos{ hash(key) };
	if (m_table[startPos] == nullptr) {
		// Not found
		node = nullptr;
		return startPos;
	}

	// Something found, compare the key to check for match
	if (m_table[startPos]->first == key) {
		// The X marks the spot!
		node =  &m_table[startPos];
		return startPos;
	}

	// We got a collision, iterate until you get the key or null
	// Get the starting position and wrap around the array looking for the key
	for (size_t i{ 0U }; i < Size; ++i) {
		size_t wrapPos{ (startPos + i) % Size };

		if (m_table[wrapPos] == nullptr) {
			// Not found
			node = nullptr;
			return wrapPos;
		}

		if (m_table[wrapPos]->first == key) {
			// The mark was off, still got the treasure
			node = &m_table[wrapPos];
			return wrapPos;
		}
	}

	// Worst case: full iteration
	node = nullptr;
	return -1;
}

#endif // !HASH_MAP_HPP


