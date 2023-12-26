// https://fluent2.microsoft.design/components/web/react/tablist/usage
// https://react.fluentui.dev/?path=/docs/components-tablist--default
// https://www.w3.org/WAI/ARIA/apg/patterns/tabs/

// https://github.com/microsoft/fluentui-system-icons
// https://master--628d031b55e942004ac95df1.chromatic.com/?path=/docs/concepts-developer-icons-icons-catalog--page

import {
    makeStyles,
    shorthands,
    Tab,
    TabList,
    TabListProps,
    Tooltip,
    TooltipProps,
} from "@fluentui/react-components";

import { Tab24Filled } from "@fluentui/react-icons";

const useStyles = makeStyles({
    root: {
        alignItems: "flex-start",
        display: "flex",
        flexDirection: "column",
        justifyContent: "flex-start",
        ...shorthands.padding("0px", "0px"),
        rowGap: "0px",
    },
});

export const Default = (props: Partial<TabListProps>) => {
    const styles = useStyles();

    return (
        <div className={styles.root}>
            <TabList {...props} appearance="subtle" size="small" vertical>
                <Tab className="bg-red-400" value="tab1">
                    <Tooltip content="Google" relationship="label">
                        <Tab24Filled />
                    </Tooltip>
                </Tab>
                <Tab value="tab2">
                    <Tab24Filled />
                </Tab>
                <Tab value="tab3">
                    <Tab24Filled />
                </Tab>
                <Tab value="tab4">
                    <Tab24Filled />
                </Tab>
            </TabList>
        </div>
    );
};
