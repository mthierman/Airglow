// https://learn.microsoft.com/en-us/microsoft-edge/webview2/reference/javascript/

interface HostObjectAsyncProxy extends HostObjectAsyncProxyBase {
    sync(): Promise<HostObjectSyncProxy>;
}

interface HostObjectAsyncProxyBase extends CallableFunction {
    applyHostFunction(argArray?: unknown): Promise<unknown>;
    getHostProperty(propertyName: string): Promise<unknown>;
    getLocalProperty(propertyName: string): unknown;
    setHostProperty(propertyName: string, propertyValue: unknown): Promise<unknown>;
    setLocalProperty(propertyName: string, propertyValue: unknown): unknown;
}

interface HostObjectSyncProxy {
    applyHostFunction(argArray?: unknown): unknown;
    async(): HostObjectAsyncProxy;
    getHostProperty(propertyName: string): unknown;
    getLocalProperty(propertyName: string): unknown;
    setHostProperty(propertyName: string, propertyValue: unknown): unknown;
    setLocalProperty(propertyName: string, propertyValue: unknown): unknown;
}

interface HostObjectsAsyncRoot extends HostObjectAsyncProxyBase {
    options: HostObjectsOptions;
    sync: HostObjectsSyncRoot;
    cleanupSome(): void;
}

interface HostObjectsOptions {
    defaultSyncProxy: boolean;
    forceAsyncMethodMatches: RegExp[];
    forceLocalProperties: string[];
    ignoreMemberNotFoundError: boolean;
    log: (...data: unknown[]) => void;
    shouldSerializeDates: boolean;
}

interface HostObjectsSyncRoot extends HostObjectSyncProxy {}

interface SharedBufferReceivedEvent extends Event {
    additionalData: unknown;
    source: WebView;
    getBuffer(): ArrayBuffer;
}

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

declare global {
    interface Window {
        chrome: {
            webview: WebView;
        };
    }
}

export type {
    HostObjectAsyncProxy,
    HostObjectAsyncProxyBase,
    HostObjectSyncProxy,
    HostObjectsAsyncRoot,
    HostObjectsOptions,
    HostObjectsSyncRoot,
    SharedBufferReceivedEvent,
    WebView,
};
