template<typename T, typename T2>
void Add(T a, T2){};

template<typename T>
void Add<T, int>(T a, int b){};