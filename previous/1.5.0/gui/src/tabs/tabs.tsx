// https://fluent2.microsoft.design/components/web/react/tablist/usage
// https://react.fluentui.dev/?path=/docs/components-tablist--default
// https://www.w3.org/WAI/ARIA/apg/patterns/tabs/

// https://github.com/microsoft/fluentui-system-icons
// https://master--628d031b55e942004ac95df1.chromatic.com/?path=/docs/concepts-developer-icons-icons-catalog--page

import { Tab, TabList, TabListProps, Tooltip } from "@fluentui/react-components";

export const Default = (props: Partial<TabListProps>) => {
    return (
        <div className="bg-red-400">
            <TabList {...props} appearance="subtle" size="small" className="bg-transparent">
                <Tab value="tab1" title="TEST" className="bg-transparent">
                    Google.com
                </Tab>
                <Tab value="tab2">
                    <Tooltip content="Google.com" relationship="label">
                        <span>Google.com</span>
                    </Tooltip>
                </Tab>
                <Tab value="tab3">Google.com</Tab>
                <Tab value="tab4">Google.com</Tab>
                <Tab value="tab5">Google.com</Tab>
                <Tab value="tab6">Google.com</Tab>
                <Tab value="tab7">Google.com</Tab>
                <Tab value="tab8">Google.com</Tab>
            </TabList>
        </div>
    );
};
