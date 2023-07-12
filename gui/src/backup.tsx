import {
    makeStyles,
    FluentProvider,
    webLightTheme,
    webDarkTheme,
    Button,
    Input,
    Label,
} from "@fluentui/react-components";
import { useEffect, useState } from "react";

const data = async () => {
        const root = "https://airglow/";
    const response = await fetch(`${root}/state.json`);
    return response.json();
}

// const useStyles = makeStyles({
//     provider: {
//         backgroundColor: "transparent",
//         height: "100%",
//     },
// });

export default function App() {
    const [data, setData] = useState([]);
    const root = "https://airglow/";

    // useEffect(() => {
    //     const dataFetch = async () => {
    //         const response = await fetch(`${root}/state.json`);
    //         return response.json();
    //         // setData(data);
    //     }});

    //     dataFetch()
    //         .then((data) => {
    //             setData(data);
    //         })
    //         .catch(console.error);
    // });

    // const dataFetch = async () => {
    //     const response = await fetch(`${root}/state.json`);
    //     const json = await response.json();
    //     return json;
    // };

    // useEffect(() => {
    //     dataFetch()
    //         .then((json) => {
    //             setData(json);
    //         })
    //         .catch(console.error);
    // });

    // const getState = async () => {
    //     const response = await fetch(`${root}/state.json`);
    //     const state = await response.json();
    //     console.log(state);
    //     console.log(state.mainUrl);
    //     console.log(state.sideUrl);

    //     mainUrl = state.mainUrl;
    //     sideUrl = state.sideUrl;
    // };

    // getState();

    // fetch("https://airglow/state.json")
    //     .then((response) => response.json())
    //     .then((json) => console.log(json));

    // window.chrome.webview.addEventListener("message", (arg: any) => {
    //     if (arg.data.includes("mainUrl")) {
    //         console.log(arg.data);
    //         mainUrl = arg.data.slice(8);
    //         console.log(mainUrl);
    //     }
    //     if (arg.data.includes("sideUrl")) {
    //         console.log(arg.data);
    //         sideUrl = arg.data.slice(8);
    //         console.log(sideUrl);
    //     }
    // });

    function handleForm(e: any) {
        e.preventDefault();
        // const data = new FormData(e.target);
        // mainUrl = Object.fromEntries(data.entries()).mainUrl;
        // sideUrl = Object.fromEntries(data.entries()).sideUrl;
        // window.chrome.webview.postMessage(`mainUrl ${mainUrl}`);
        // window.chrome.webview.postMessage(`sideUrl ${sideUrl}`);
    }

    const styles = useStyles();

    return (
        // <FluentProvider className={styles.provider} theme={webDarkTheme}>
        //     <form
        //         autoComplete="off"
        //         id="settings"
        //         method="post"
        //         onSubmit={handleForm}>
        //         <div id="url">
        //             <Label>
        //                 Test
        //                 <Input
        //                     type="url"
        //                     name="mainUrl"
        //                     placeholder={data.results[0]}
        //                     pattern="https://.*"
        //                     required></Input>
        //             </Label>

        //             <Label>
        //                 Test
        //                 <Input
        //                     type="url"
        //                     name="sideUrl"
        //                     placeholder=""
        //                     pattern="https://.*"
        //                     required></Input>
        //             </Label>
        //         </div>

        //         <Button appearance="primary" type="submit">
        //             Save
        //         </Button>
        //     </form>
        // </FluentProvider>
    );
}
b
