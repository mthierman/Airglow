import { Tab, TabList, TabListProps } from "@fluentui/react-components";

export const Default = (props: Partial<TabListProps>) => {
    return (
        <TabList {...props} appearance="subtle" size="small" vertical>
            <Tab value="tab1" title="https://www.google.com/">
                Google.com
            </Tab>
            <Tab value="tab2" title="https://www.google.com/">
                Google.com
            </Tab>
            <Tab value="tab3" title="https://www.google.com/">
                Google.com
            </Tab>
            <Tab value="tab4" title="https://www.google.com/">
                Google.com
            </Tab>
            <Tab value="tab5" title="https://www.google.com/">
                Google.com
            </Tab>
            <Tab value="tab6" title="https://www.google.com/">
                Google.com
            </Tab>
            <Tab value="tab7" title="https://www.google.com/">
                Google.com
            </Tab>
            <Tab value="tab8" title="https://www.google.com/">
                Google.com
            </Tab>
            <Tab value="tab9" title="https://www.google.com/">
                Google.com
            </Tab>
        </TabList>
    );
};
