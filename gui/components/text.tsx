import { useEffect, useState, useRef } from "react";

export default function Text() {
    let container = useRef(null);
    let [height, setHeight] = useState(null);

    useEffect(() => {
        setHeight(container.current.offsetHeight);
        if (height) console.log(height);
    });

    return (
        <div ref={container}>
            <p>
                Morbi nulla pretium tincidunt porttitor convallis malesuada varius a dignissim cum
                sem. Fermentum nostra malesuada sollicitudin malesuada. In class dictumst urna eget
                natoque accumsan ad. Class, habitasse hendrerit tempus erat. Quis cursus nibh
                malesuada litora feugiat magna libero accumsan. Semper hac eget varius vivamus.
                Sollicitudin ultrices augue sodales urna curae; vel eros eros ullamcorper nullam
                feugiat mauris. Rutrum dui curabitur donec venenatis malesuada aenean primis mus
                vivamus at! Montes consectetur phasellus mi nam ad hendrerit consectetur porttitor
                nisl ipsum rutrum vulputate. Velit penatibus massa mus et.
            </p>
            <p>
                Ligula curae; semper volutpat sociis. Pellentesque eget sagittis luctus nisi
                lobortis duis. Iaculis nam sociosqu eleifend fermentum commodo morbi fusce dictum
                phasellus cubilia! Cum, scelerisque purus aliquam velit eu. Venenatis quis lectus
                quis lacinia per sociosqu varius torquent. Scelerisque sollicitudin, odio sed.
                Sodales placerat, imperdiet class netus ridiculus nisl sapien sociis. Ornare
                dignissim cum condimentum nibh fusce suscipit fringilla ultrices nisl. Ad rutrum
                facilisi dictumst aenean natoque. Nam platea sed.
            </p>
            <p>
                Urna duis porttitor volutpat nulla magna litora. Morbi ipsum proin dictumst donec
                tempor ultrices elit. Maecenas nisl vehicula tincidunt nullam aliquam sit ornare at
                tempus posuere maecenas? Suscipit faucibus velit metus conubia. Ornare est lectus
                mauris sollicitudin. Proin aliquet ligula euismod orci leo pretium himenaeos
                posuere! Sociosqu venenatis potenti congue mi in fames potenti vitae suspendisse.
                Dictum imperdiet mi semper sociis tristique. Metus fringilla vulputate facilisis
                magnis curae; mus! Cursus class aptent turpis donec proin blandit tellus vehicula
                semper facilisi posuere? Curabitur.
            </p>
            <p>
                At phasellus habitant velit massa. Est per quam magna integer iaculis dictumst
                cursus phasellus. Hendrerit eu eros sit vulputate velit potenti nisi. Dignissim
                vestibulum at commodo, non libero quisque semper imperdiet fermentum vestibulum
                pulvinar turpis. Praesent ornare condimentum mi congue est sagittis tempus aliquet
                habitant torquent. Feugiat metus rutrum elementum proin consequat.
            </p>
            <p>
                Donec integer suscipit at per, metus commodo quis cum? Condimentum curae; senectus
                gravida ad pretium dui pretium congue amet erat! Accumsan ligula penatibus
                pellentesque faucibus molestie mauris himenaeos turpis. Rutrum laoreet viverra
                porttitor bibendum. Lorem feugiat cras curabitur porttitor senectus quisque
                placerat. Litora curabitur suscipit consequat quisque himenaeos odio rhoncus.
            </p>
            <p>
                Sit lacinia fermentum volutpat est enim, potenti mi et. Purus, eget urna volutpat
                blandit. Sagittis ridiculus amet mi. Cum nunc penatibus nam at faucibus integer.
                Placerat suscipit lobortis facilisi turpis laoreet cras hac nam natoque sollicitudin
                condimentum mattis! Cursus velit, euismod eu. Fames quisque parturient volutpat
                cursus orci primis praesent.
            </p>
            <p>
                Id ante nascetur mollis ac urna viverra urna lobortis lectus quis. Cum penatibus in
                aliquam convallis auctor libero faucibus. Quisque dapibus auctor quisque cubilia
                fermentum a fringilla per lobortis. Porta et penatibus cubilia sit amet parturient,
                magna sit per. Netus justo magna fermentum erat dapibus nullam class blandit integer
                tortor porttitor diam. Hendrerit non consequat urna duis.
            </p>
            <p>
                Tincidunt, parturient nulla eros! Nam enim pellentesque, rhoncus auctor ut.
                Imperdiet nam donec massa cubilia fusce posuere duis senectus! Varius praesent
                feugiat duis nascetur molestie litora interdum consectetur litora sapien.
                Condimentum feugiat iaculis magna. Felis donec habitant metus, auctor senectus!
                Vestibulum sollicitudin sociis cubilia nec ac sollicitudin. Sodales habitant
                penatibus nostra tempus elit habitasse. Cubilia tincidunt facilisi pharetra.
                Eleifend purus penatibus felis dictumst habitant ut platea laoreet tortor. Purus nam
                sociis velit congue erat ullamcorper magnis maecenas auctor rhoncus.
            </p>
            <p>
                Etiam, hendrerit litora class nulla vitae massa id montes ridiculus libero. Arcu
                aenean consectetur adipiscing nullam posuere conubia mi in fames class. Venenatis
                nec bibendum sociosqu suspendisse mattis. Vitae, lacus amet lacus natoque nam
                convallis scelerisque luctus adipiscing. Etiam lorem egestas facilisis. Tempus
                commodo pulvinar ante eros vel vehicula rhoncus leo id pulvinar. Curabitur luctus
                mattis dui vivamus volutpat feugiat tellus vel dictumst. Interdum elementum sagittis
                sed. Netus tristique ad sed aptent placerat malesuada nullam odio. Netus elementum
                massa mus pretium.
            </p>
            <p>
                Egestas aenean enim nisl ut taciti ultrices, ridiculus sodales. Tristique cursus
                feugiat ultricies. Eu ullamcorper risus eleifend dapibus risus accumsan? Facilisi
                malesuada, aliquet aenean sed sagittis. Dignissim elementum litora facilisis
                senectus ipsum vulputate tortor. Pellentesque feugiat proin quis mi nascetur
                natoque, torquent condimentum quam sagittis. Dolor bibendum ac curae; neque eros eu.
                Erat vitae augue facilisis. Laoreet, tincidunt in nullam sodales vestibulum mi!
                Ipsum adipiscing natoque lacinia nostra euismod auctor. Et arcu suscipit aliquet
                eget consequat habitant eros dui dignissim urna hac ultricies. Id molestie netus
                molestie sociosqu?
            </p>
            <p>
                Sollicitudin imperdiet a non a diam arcu tincidunt lorem velit. Torquent orci in
                elit amet vivamus. Accumsan tortor ad hac eleifend lacinia dictumst lobortis
                dignissim. Nec quis nec rhoncus elit sociosqu sem congue! Feugiat imperdiet
                consequat velit nisi dictumst facilisis ornare praesent. Accumsan consectetur aenean
                egestas, orci purus lobortis amet adipiscing taciti vulputate. Donec nullam conubia
                mus duis vel venenatis aliquet. Turpis dictumst habitasse.
            </p>
            <p>
                Viverra dolor potenti dui natoque varius nam vulputate id elit eget magna
                suspendisse! Magna nisl consequat a dapibus dolor pretium euismod vulputate nibh
                ornare! Mus diam erat urna condimentum porttitor vehicula? Quam consectetur mollis
                senectus. Ipsum blandit tristique nunc integer parturient. Natoque mauris commodo
                habitant nunc augue convallis laoreet pellentesque tristique torquent, ultricies
                magnis. Felis malesuada nascetur dapibus phasellus velit luctus diam donec mattis
                scelerisque. Mauris ultrices.
            </p>
            <p>
                Sollicitudin purus urna blandit libero est suscipit consectetur pulvinar urna
                lectus. Massa rutrum tempus sodales pretium nulla suspendisse. Eu inceptos nullam
                adipiscing velit a orci eros felis inceptos. Vestibulum senectus cubilia netus
                imperdiet molestie vivamus. Turpis tellus magnis nascetur pellentesque nisl nostra
                praesent cubilia. Mus conubia turpis tellus egestas risus ultricies malesuada
                egestas odio! Interdum elementum fermentum dolor nunc eu cum. Eros suscipit mattis
                urna purus felis porta! Ad integer malesuada parturient eget. Natoque fringilla hac
                egestas inceptos.
            </p>
            <p>
                Elementum sed suscipit duis, vivamus porta sit sociis libero diam dignissim. Rhoncus
                elit, natoque aliquet convallis? Pellentesque adipiscing litora per fermentum sed
                arcu, viverra penatibus donec nisi hac. Posuere porta in aenean enim integer taciti
                dictum mollis dignissim. Aliquam leo libero bibendum, vitae dignissim semper rhoncus
                platea phasellus torquent cras. Quam rhoncus eu ullamcorper laoreet himenaeos. Nam
                ac in laoreet netus habitant. A vulputate ultrices libero vitae mi lobortis ipsum
                est. A porttitor libero felis sociis sapien.
            </p>
            <p>
                Pharetra ultricies dictumst elit. Senectus leo turpis phasellus malesuada rhoncus
                egestas suscipit rutrum auctor. Adipiscing semper risus potenti dolor ante fringilla
                dictum dui leo est. Malesuada vitae vivamus curabitur. Volutpat nec nam mus amet
                viverra metus placerat fermentum est nec. Facilisi natoque ultricies lacus aptent
                tellus sapien aptent sed eleifend viverra amet? Accumsan cursus praesent et aliquam
                aliquet at luctus fermentum tristique. Ultricies cursus ligula consectetur
                consectetur natoque luctus euismod litora ipsum volutpat eu ac. Auctor, pulvinar
                taciti.
            </p>
        </div>
    );
}
