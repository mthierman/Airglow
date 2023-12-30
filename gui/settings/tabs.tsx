import {
    Tab,
    TabList,
    TabListProps,
    makeStyles,
    tokens,
    shorthands,
} from "@fluentui/react-components";

const useStyles = makeStyles({
    root: {
        color: tokens.colorNeutralStrokeOnBrand2,
        ...shorthands.padding("8px"),
    },
});

export const Default = (props: Partial<TabListProps>) => {
    const classes = useStyles();
    const overrides = useStyles();

    return (
        <div className={classes.root}>
            test
            <TabList {...props} appearance="subtle" size="small" vertical>
                <Tab className={overrides.root} value="tab1" title="https://www.google.com/">
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
        </div>
    );
};
