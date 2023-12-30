import { useEffect, useState, useRef } from "react";

import { Default as Tabs } from "./tabs";

export default function App() {
    let container = useRef(null);
    let [height, setHeight] = useState(null);

    useEffect(() => {
        setHeight(container.current.offsetHeight);
        if (height) console.log(height);
    });

    return (
        <div ref={container}>
            <Tabs />
        </div>
    );
}

// // https://stackoverflow.com/questions/37265451/react-dom-offsetheight-before-rendering
// document.onreadystatechange = () => {
//     if (document.readyState === "complete") {
//         const root = document.body;
//         console.log(root.offsetHeight);
//     }
// };
