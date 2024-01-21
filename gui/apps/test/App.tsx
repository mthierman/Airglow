import { SyntheticEvent, useState, useLayoutEffect, useRef, useEffect } from "react";
import * as url from "@libs/url";
import { getSessionStorage, getPositionStorage, getSystemColorsStorage } from "@libs/storage";
import { Button } from "@components/Button";

export default function App() {
    return <>
        <Button text="test" />
    </>;
}
