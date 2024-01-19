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
        interface URL {
            current: string;
            loaded: string;
        }

        interface Home {
            current: string;
        }

        interface Position {
            bar: number;
            border: number;
            horizontal: boolean;
            split: boolean;
            swapped: boolean;
        }
    }
}
