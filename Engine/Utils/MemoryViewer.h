#pragma once

#include "Core/Base.h"

namespace zn
{
	template<typename T>
	class MemoryViewer
	{
	public:
		using ValueType = T;
		using DataType = const uint8_t*;
		using SizeType = std::size_t;
		using WidthType = SizeType;

		constexpr explicit MemoryViewer(const T* obj) noexcept
		{
			m_Data = reinterpret_cast<DataType>(obj);

			m_Lines = MemoryViewer::CalculateLines(m_Size, m_Width);
		};

		constexpr explicit MemoryViewer(const T* obj, SizeType size) noexcept
		{
			m_Data = reinterpret_cast<DataType>(obj);

			m_Size = size;

			m_Lines = MemoryViewer::CalculateLines(m_Size, m_Width);
		};

		~MemoryViewer() { m_Data = nullptr; };

		void Print() const noexcept
		{
			ZN_CORE_ASSERT(m_Data != nullptr)

			std::printf("\n");
			std::printf(" ===================================================================\n");
			std::printf(" ADDRESS          || 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n");
			std::printf(" ================ || ===============================================\n");

			DataType p = m_Data;
			for (SizeType i{}; i < m_Lines; ++i)
			{
				PrintMemoryLine(p);
				p += m_Width;
			}

			std::printf(" ===================================================================\n");
			std::printf("\n");
		}

	private:
		[[nodiscard]]
		size_t CalculateLines(const SizeType size, const SizeType width = 16) const noexcept
		{
			const auto lines{ size / width };

			//mod of lines
			if ((lines & 0xF) != 0)
				return lines + 1;

			return lines;
		}

		void PrintMemoryLine(DataType lineData) const noexcept
		{
			std::printf(" %16p ||", (void*)lineData);

			for (SizeType i{}; i < m_Width; ++i)
			{
				std::printf(" %02X", lineData[i]);
			}

			std::printf(" || ");

			for (SizeType i{}; i < m_Width; ++i)
			{
				char c = (lineData[i] > 31 && lineData[i] < 128) ? lineData[i] : '.';

				std::printf("%c", c);
			}

			std::printf(" || \n");
		}

	private:
		DataType m_Data = nullptr;

		SizeType m_Size{ sizeof(T) };
		SizeType m_Width{ 16 };
		SizeType m_Lines;
	};
}