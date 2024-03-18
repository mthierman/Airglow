type pair = [first: string, second: string];

interface Colors {
    accent: string;
    accentDark1: string;
    accentDark2: string;
    accentDark3: string;
    accentLight1: string;
    accentLight2: string;
    accentLight3: string;
}

interface Layout {
    bar: number;
    border: number;
    focus: string;
    init: boolean;
    split: boolean;
    swap: boolean;
    vertical: boolean;
}

interface State {
    args: pair;
    colors: Colors;
    home: pair;
}

interface Page {
    favicon: string;
    source: string;
    title: string;
}

interface AppSettings {
    m_state: State;
}

interface AppWindow {
    m_first: Page;
    m_layout: Layout;
    m_second: Page;
    m_state: State;
    m_url: Page;
    focus: string;
    navigate: pair;
}

export type { AppSettings, AppWindow, Colors, Layout, Page, State, pair };
