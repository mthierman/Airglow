import { Tab, TabList, TabListProps } from "@fluentui/react-components";

export const Default = (props: Partial<TabListProps>) => {
    return (
        <TabList {...props} appearance="subtle" size="small">
            <Tab value="tab1" title="https://www.google.com/">
                Google.com
            </Tab>
        </TabList>
    );
};
