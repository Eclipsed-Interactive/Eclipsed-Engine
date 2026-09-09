#pragma once

#define BASIC_VIEW(PrettyName)														\
public:																			\
	static constexpr const char* GetStaticName() { return PrettyName; }

namespace Eclipse::Editor
{
	class IEditorView
	{
	public:
		virtual ~IEditorView() = default;

		virtual void Draw() = 0;

		virtual void OnOpen() {}
		virtual void OnClose() {}

		virtual const char* GetName() const = 0;
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