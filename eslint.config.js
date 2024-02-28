// @ts-check

import eslint from "@eslint/js";
import eslintPluginReact from "eslint-plugin-react";
import eslintPluginReactHooks from "eslint-plugin-react-hooks";
import globals from "globals";
import tseslint from "typescript-eslint";

export default tseslint.config(
    eslint.configs.recommended,
    ...tseslint.configs.recommendedTypeChecked,
    {
        plugins: {
            "eslint-plugin-react": eslintPluginReact,
            "eslint-plugin-react-hooks": eslintPluginReactHooks,
        },
        languageOptions: {
            parserOptions: {
                project: ["tsconfig.json", "tsconfig.node.json"],
                tsconfigRootDir: import.meta.dirname,
            },
        },
    },
    {
        files: ["**/*.js"],
        languageOptions: {
            globals: {
                ...globals.node,
            },
        },
        ...tseslint.configs.disableTypeChecked,
    },
);
