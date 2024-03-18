// https://learn.microsoft.com/en-us/microsoft-edge/webview2/reference/javascript/webview

interface WebView extends EventTarget {
    hostObjects: HostObjectsAsyncRoot;
    addEventListener(
        type: string,
        listener: EventListenerOrEventListenerObject,
        options?: boolean | AddEventListenerOptions,
    ): void;
    postMessage(message: unknown): void;
    postMessageWithAdditionalObjects(message: unknown, additionalObjects: ArrayLike<unknown>): void;
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
}
