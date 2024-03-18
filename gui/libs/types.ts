interface AppColors {
    accent: string;
    accentDark1: string;
    accentDark2: string;
    accentDark3: string;
    accentLight1: string;
    accentLight2: string;
    accentLight3: string;
}

interface AppLayout {
    bar: number;
    border: number;
    focus: string;
    init: boolean;
    split: boolean;
    swap: boolean;
    vertical: boolean;
}

interface AppSettings {
    m_state: AppState;
}

interface AppState {
    args: pair;
    colors: AppColors;
    home: pair;
}

interface AppWindow {
    m_first: Page;
    m_layout: AppLayout;
    m_second: Page;
    m_state: AppState;
    m_url: Page;
    focus: string;
    navigate: pair;
}

interface Page {
    favicon: string;
    source: string;
    title: string;
}

type pair = [first: string, second: string];

export type { AppColors, AppLayout, AppSettings, AppState, AppWindow, Page, pair };
