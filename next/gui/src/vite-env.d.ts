/// <reference types="vite/client" />

declare module "*.svg?raw" {
    const value: string;
    export default value;
}
