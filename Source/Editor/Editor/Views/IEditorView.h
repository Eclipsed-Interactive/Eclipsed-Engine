#pragma once

#include <string_view>
#include <string>

#define BASIC_VIEW(PrettyName)														\
public:																			\
	static constexpr const char* GetStaticName() { return PrettyName; }

namespace Eclipse::Editor
{
	class IEditorView
	{
	public:
		virtual ~IEditorView() = default;

		void SetID(std::string id);

		virtual void Draw() = 0;

		virtual void OnOpen() {}
		virtual void OnClose() {}

		virtual const char* GetName() const = 0;

	protected:
		void Indent(int indent);
		void Unindent(int indent);

	protected:
		std::string viewId;
		int currentIndent = 0;

	public:
		unsigned ActiveImGuiFlag = 0;
	};

	template<typename T>
	class EditorView : public IEditorView
	{
	public:
		virtual ~EditorView() = default;

	private:
		static inline constexpr const char* GetStaticName()
		{
			return T::GetStaticName();
		}

		inline const char* GetName() const override
		{
			return T::GetStaticName();
		}
	};
}