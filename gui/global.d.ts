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

    namespace App {
        interface Pair {
            first: string;
            second: string;
        }

        interface Layout {
            bar: number;
            border: number;
            horizontal: boolean;
            split: boolean;
            swapped: boolean;
        }

        interface Dimensions {
            devicePixelRatio: number;
            offsetHeight: number;
            offsetWidth: number;
        }

        interface Colors {
            accent: string;
            accentDark1: string;
            accentDark2: string;
            accentDark3: string;
            accentLight1: string;
            accentLight2: string;
            accentLight3: string;
        }

        interface Window {
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

        interface Settings {
            m_colors: {
                colors: Colors;
            };
            m_url: {
                current: [string, string];
                home: [string, string];
            };
        }
    }
}
