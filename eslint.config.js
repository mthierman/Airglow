// @ts-check

import eslint from "@eslint/js";
import reactPlugin from "eslint-plugin-react";
import hooksPlugin from "eslint-plugin-react-hooks";
import eslintPluginReactRecommended from "eslint-plugin-react/configs/recommended.js";
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
            "react-hooks": hooksPlugin,
        },
        rules: {
            ...eslintPluginReactRecommended.rules,
            ...hooksPlugin.configs.recommended.rules,
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
        files: ["**/*.config.ts"],
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
