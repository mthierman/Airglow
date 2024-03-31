// @ts-check

import eslint from "@eslint/js";
import reactPlugin from "eslint-plugin-react";
import reactHooksPlugin from "eslint-plugin-react-hooks";
import reactPluginConfigsJsxRuntime from "eslint-plugin-react/configs/jsx-runtime.js";
import reactPluginConfigsRecommended from "eslint-plugin-react/configs/recommended.js";
import globals from "globals";
import tseslint from "typescript-eslint";

export default tseslint.config(
    eslint.configs.recommended,
    ...tseslint.configs.recommendedTypeChecked,
    {
        ignores: ["**/build/**"],
    },
    {
        plugins: {
            "react": reactPlugin,
            "react-hooks": reactHooksPlugin,
        },
        rules: {
            ...reactPluginConfigsRecommended.rules,
            ...reactPluginConfigsJsxRuntime.rules,
            ...reactHooksPlugin.configs.recommended.rules,
        },
        languageOptions: {
            globals: {
                ...globals.browser,
            },
            parserOptions: {
                project: ["tsconfig.json"],
                tsconfigRootDir: import.meta.dirname,
                ecmaFeatures: {
                    jsx: true,
                },
            },
        },
    },
    {
        files: ["**/*.config.ts", "**/tools/scripts/*.ts"],
        languageOptions: {
            globals: {
                ...globals.node,
            },
            parserOptions: {
                project: ["tsconfig.node.json"],
                tsconfigRootDir: import.meta.dirname,
            },
        },
    },
    {
        files: ["**/*.config.js"],
        languageOptions: {
            globals: {
                ...globals.node,
            },
        },
        ...tseslint.configs.disableTypeChecked,
    },
);
