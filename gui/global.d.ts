// https://learn.microsoft.com/en-us/microsoft-edge/webview2/reference/javascript/webview

interface WebView extends EventTarget {
    hostObjects: HostObjectsAsyncRoot;
    addEventListener(
        type: string,
        listener: EventListenerOrEventListenerObject,
        options?: boolean | AddEventListenerOptions,
    ): void;
    postMessage(message: any): void;
    postMessageWithAdditionalObjects(message: any, additionalObjects: ArrayLike<any>): void;
    releaseBuffer(buffer: ArrayBuffer): void;
    removeEventListener(
        type: string,
        listener: EventListenerOrEventListenerObject,
        options?: boolean | EventListenerOptions,
    ): void;
}

export declare global {
    interface Window {
        chrome: {
            webview: WebView;
        };
    }

    type Pair = [first: string, second: string];

    interface PairObj {
        first: string;
        second: string;
    }

    namespace App {
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
            args: Pair;
            colors: Colors;
            home: Pair;
        }

        interface Settings {
            m_state: State;
        }

        interface Window {
            m_state: State;
            m_colors: {
                colors: Colors;
            };
            m_faviconUrl: [string, string];
            m_focus: string;
            m_layout: Layout;
            m_url: {
                current: [string, string];
                home: [string, string];
            };
            focus: string;
            navigate: [string, string];
        }
    }
}
