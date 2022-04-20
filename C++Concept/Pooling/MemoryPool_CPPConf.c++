#include <cstdint>
#include <cstddef>
#include <iostream>
#include <cassert>

using namespace std;

class bucket {
public :
	const std::size_t BlockSize;
	const std::size_t BlockCount;

	bucket(std::size_t Block_Size, std::size_t Block_Count);
	~bucket();

	// 해당 Pointer 가 현재 Bucket 에 속하는지 확인한다.
	bool Belongs(void* ptr) const noexcept;
	// Returns Nullptr If Failed
	void* Allocate(std::size_t bytes) noexcept;
	void Deallocate(void* Ptr, std::size_t bytes) noexcept;
private :
	// Finds n free contiguous blocks in ledger , return first block's index or BlockCount on Failure
	std::size_t find_contiguous_blocks(std::size_t n) const noexcept;
	// 'index' 위치에서 시작하여, n 개의 Block 만큼 "in-use"로 표시
	void set_blocks_in_use(std::size_t index, std::size_t n) noexcept;
	void set_blocks_free(std::size_t index, std::size_t n) noexcept;

private :
	// Actual Memory For Allocation
	std::byte* m_data{ nullptr };

	// 하나의 bit 가, 각 Block 이 할당되었는지 아닌지에 대한 정보를 지니고 있게 된다.
	std::byte* m_ledger{ nullptr };
};

bucket::bucket(std::size_t Block_Size, std::size_t Block_Count) :
BlockSize(Block_Size),
BlockCount(Block_Count)
{
	const auto data_size = Block_Size * BlockCount;
	m_data = (std::byte*)std::malloc(data_size);
	assert(m_data != nullptr);

	const auto ledger_size = 1 + (BlockCount - 1) / 8;
	m_ledger = (std::byte*)(std::malloc(ledger_size));
	assert(m_ledger != nullptr);

	std::memset(m_data, 0, data_size);
	std::memset(m_ledger, 0, ledger_size);
}

bucket::~bucket()
{
	std::free(m_ledger);
	std::free(m_data);	
}

bool bucket::Belongs(void* ptr) const noexcept
{}

void* bucket::Allocate(std::size_t bytes) noexcept
{
	// Calculate Required Number Of Blocks
	const auto n = 1 + ((bytes - 1) / BlockSize);

	const auto index = find_contiguous_blocks(n);

	if (index == BlockCount)
		return nullptr;

	// index 위치부터, n byte ? 만큼 사용 중이라고 표시한다.
	set_blocks_in_use(index, n);

	// 할당된 메모리 영역의 시작 주소값을 반환한다.
	return m_data + (index * BlockSize);
}

void bucket::Deallocate(void* Ptr, std::size_t bytes) noexcept
{
	// Ptr : 포인터
	const auto p = (const std::byte*)Ptr;

	const std::size_t dist = (std::size_t)(p - m_data);

	// Calculate Block Index from Pointer Distance
	const auto index = dist / BlockSize;

	// Calculate Required Number Of Blocks
	const auto n = 1 + ((bytes - 1) / BlockSize);

	// Update Ledger
	set_blocks_free(index, n); 
}

std::size_t bucket::find_contiguous_blocks(std::size_t n) const noexcept
{}

void bucket::set_blocks_in_use(std::size_t index, std::size_t n) noexcept
{}

void bucket::set_blocks_free(std::size_t index, std::size_t n) noexcept
{}

// Bucket 의 BlockSize, BlockCount 라는 매개변수들이, 해당 Pool Instance를 결정하는 요소가 된다.

// Default Implementation Defined A Pool With No Buckets
template<std::size_t id>
struct bucket_descriptors {
	using type = std::tuple<>;
};

struct bucket_cfg16 {
	static constexpr std::size_t BlockSize = 16;
	static constexpr std::size_t BlockCount = 10000;
};

struct bucket_cfg32 {
	static constexpr std::size_t BlockSize = 32;
	static constexpr std::size_t BlockCount = 10000;
};

struct bucket_cfg1024 {
	static constexpr std::size_t BlockSize = 1024;
	static constexpr std::size_t BlockCount = 50000;
};

template<>
struct bucket_descriptors<1> {
	using type = std::tuple<bucket_cfg16, bucket_cfg32, bucket_cfg1024>;
};

// 결국 id 를 가진 bucket 인수 목록들의 "tuple"을 가져다주는 녀석이다.
template<std::size_t id>
using bucket_descriptors_t = typename bucket_descriptors<id>::type;

// std::tuple_size<std::tuple> : number of elements in tuple in compile_time
template<std::size_t id>
static constexpr std::size_t bucket_count = std::tuple_size<bucket_descriptors_t<id>>::value;

// ex) 3개의 Bucket 을 만들어낸다.
template<std::size_t id>
using pool_type = std::array<bucket, bucket_count<id>>;

// ex) std::integral_constant<int, 2> --> int type ?에 2 라는 value 객체를 지닌, static class ?를 리턴한다.
template<std::size_t id, std::size_t Idx>
struct get_size :
	std::integral_constant<
		std::size_t,
		// std::tuple_element : type of Ith Element In Tuple
		std::tuple_element_t<Idx, bucket_descriptors_t<id>>::BlockSize>{};

template<std::size_t id, std::size_t Idx>
struct get_count
	: std::integral_constant < std::size_t, std::tuple_element_t<Idx, bucket_descriptors_t<id>>::BlockCount>{} ;

// Define Pool
template<std::size_t id, std::size_t ...Idx>
// std::index_sequence 는 Parameter Pack 을 deduce(추론)하기 위해 사용된다.
// 컴파일 Tile 때 Integer의 Sequence 들을 나타낸다.
// ex) 4가 들어오면 --> 0,1,2,3 으로 만들어준다...?
auto& get_instance(std::index_sequence<Idx...>) noexcept
{
	// pool_type<id> --> std::array 를 return 해준다.
	// --> Idx 템플릿 매개변수 개수만큼의 bucket 를 담은 array 를 리턴해준다.
	static pool_type<id> instance{ {{get_size<id, Idx>::value, get_count<id, Idx>::value} ...} };

	return instance;
}

template<std::size_t id>
auto& get_instance() noexcept
{
	// bucket_count --> id 라는 id ? 를 지닌,  "bucket 들에 대한 인자"를 담은 "tuple", 그 안의 "인자의 개수"
	// std::make_index_sequence 의 경우, std::index_sequence<Ints...> 형태로 들어가게 되는 것이다.
	return get_instance<id>(std::make_index_sequence<bucket_count<id>>());
}

// 충분히 큰 Bucket 이 존재한다면 해당 Bucket을 바로 리턴한다
// First Fit Strategy
template<std::size_t id>
void* allocate (std::size_t bytes)
{
	auto& pool = get_instance<id>();

	for (auto& bucket : pool)
	{
		if (bucket.BlockSize >= bytes)
		{
			if (auto ptr = bucket.allocate(bytes); ptr != nullptr)
			{
				return ptr;
			}
		}
	}

	// new 연산자를 통한 메모리 할당이 실패할 경우, 예외가 발생하여 프로그램 즉시 종료
	throw std::bad_alloc();
}

struct info {
	std::size_t index{0}; // Which Bucket
	std::size_t block_count{ 0 }; // How Many Blocks Would The Allocation  Take From The Bucket ?
	std::size_t waste{ 0 }; // How Much Memory Would Be Wasted ?

	// 
	bool operator < (const info& other) const noexcept
	{
		return (waste == other.waste) ? block_count < other.block_count : waste < other.waste;
	}
};

int main()
{
	return 0;
}
