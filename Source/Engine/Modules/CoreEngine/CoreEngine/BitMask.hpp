#pragma once

namespace Eclipse
{
	namespace Utilities
	{
		// Gives you a default of 32 bits.
		template <typename T = int>
		class BitMask
		{
		public:
			BitMask();
			BitMask(const T& aOther);

		public:
			bool GetBit(const int& aBit) const;
			void SetBit(const int& aBit, const bool& aState);

			void SetAll(const bool& aState);

			const T& GetMask();

			static bool GetBit(const int& aValue, const int& aBit);

			void operator=(const BitMask<T>& other) { myBits = other.myBits; }
			void operator=(const T& other) { myBits = other; }

		private:
			const int myCapacity = -1;
			T myBits = 0;
		};

		template<typename T>
		inline BitMask<T>::BitMask() : myCapacity(sizeof(T) * 8)
		{}

		template<typename T>
		inline BitMask<T>::BitMask(const T & aOther)
		{
			myBits = aOther;
		}

		template<typename T>
		inline bool BitMask<T>::GetBit(const int& aBit) const
		{
			return myBits & (1UL << aBit);
		}

		template<typename T>
		inline void BitMask<T>::SetBit(const int& aBit, const bool& aState)
		{
			if (aBit >= myCapacity) static_assert("Bit to high.");

			if (aState)
			{
				myBits |= (1UL << aBit);
			}
			else
			{
				myBits &= ~(1UL << aBit);
			}
		}

		template<typename T>
		inline void BitMask<T>::SetAll(const bool& aState)
		{
			for (int i = 0; i < myCapacity; i++)
			{
				if (aState)
				{
					myBits |= (1UL << i);
				}
				else
				{
					myBits &= ~(1UL << i);
				}
			}
		}
		template<typename T>
		inline const T& BitMask<T>::GetMask() { return myBits; }
		template<typename T>
		inline bool BitMask<T>::GetBit(const int& aValue, const int& aBit)
		{
			return aValue & (1UL << aBit);
		}
	}

	namespace Util = Utilities;

}

