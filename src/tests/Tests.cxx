#include "../Utility.hxx"

#ifdef _DEBUG
#include <print>
#endif

void run_tests()
{
#ifdef _DEBUG
    std::println("{}",
                 util::system_color(winrt::Windows::UI::ViewManagement::UIColorType::Foreground));
    std::println("{}",
                 util::system_color(winrt::Windows::UI::ViewManagement::UIColorType::Background));

    util::bool_toggle(false) ? std::println("bool_toggle(): PASS")
                             : std::println("bool_toggle(): FAIL");
#endif
}

int main()
{
    run_tests();

    return 0;
}
