#pragma once

template <typename T>
class Scope {
private:
	T* Pointer;
public:
	T* Get() {
		return this->Pointer;
	}
	void Set(T* Pointer) {
		this->Pointer = Pointer;
	}
	void Delete() {
		delete this->Pointer;
	}
	template<typename Result>
	Result* Cast() {
		return reinterpret_cast<Result*>(this->Pointer);
	}
	static void Create(Scope& scope) {
		scope = Scope();
		scope.Pointer = new T();
	}
};