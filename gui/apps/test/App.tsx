import { SyntheticEvent, useState, useLayoutEffect, useRef, useEffect } from "react";
import * as url from "@libs/url";
import { getSessionStorage, getPositionStorage, getSystemColorsStorage } from "@libs/storage";
import { Button } from "@components/Button";

export default function App() {
    return (
        <>
            <Button text="test" />
            <Button text="test" />
            <Button text="test" />
            <Button text="test" />
            <p>
                Lorem ipsum dolor sit amet, consectetur adipiscing elit. Maecenas tincidunt euismod
                tortor eget eleifend. Mauris porttitor tortor vel neque condimentum commodo. Aliquam
                ornare posuere enim, quis laoreet magna faucibus vitae. Maecenas posuere hendrerit
                lectus, at dignissim magna finibus eu. Sed vulputate euismod ornare. Aenean eleifend
                sed mi quis bibendum. Etiam imperdiet elit dui, et eleifend elit auctor molestie.
                Aenean sodales, libero quis dictum maximus, massa sem rutrum risus, non convallis
                lorem mi sed felis. Etiam sapien arcu, finibus at ipsum at, maximus feugiat magna.
                Pellentesque posuere purus non risus aliquam, ac imperdiet nulla porttitor. Aliquam
                vel est quis metus viverra malesuada facilisis eu ex. Mauris aliquet hendrerit
                neque, nec pharetra magna tristique feugiat.
            </p>
            <p>
                Lorem ipsum dolor sit amet, consectetur adipiscing elit. Maecenas tincidunt euismod
                tortor eget eleifend. Mauris porttitor tortor vel neque condimentum commodo. Aliquam
                ornare posuere enim, quis laoreet magna faucibus vitae. Maecenas posuere hendrerit
                lectus, at dignissim magna finibus eu. Sed vulputate euismod ornare. Aenean eleifend
                sed mi quis bibendum. Etiam imperdiet elit dui, et eleifend elit auctor molestie.
                Aenean sodales, libero quis dictum maximus, massa sem rutrum risus, non convallis
                lorem mi sed felis. Etiam sapien arcu, finibus at ipsum at, maximus feugiat magna.
                Pellentesque posuere purus non risus aliquam, ac imperdiet nulla porttitor. Aliquam
                vel est quis metus viverra malesuada facilisis eu ex. Mauris aliquet hendrerit
                neque, nec pharetra magna tristique feugiat.
            </p>
            <p>
                Lorem ipsum dolor sit amet, consectetur adipiscing elit. Maecenas tincidunt euismod
                tortor eget eleifend. Mauris porttitor tortor vel neque condimentum commodo. Aliquam
                ornare posuere enim, quis laoreet magna faucibus vitae. Maecenas posuere hendrerit
                lectus, at dignissim magna finibus eu. Sed vulputate euismod ornare. Aenean eleifend
                sed mi quis bibendum. Etiam imperdiet elit dui, et eleifend elit auctor molestie.
                Aenean sodales, libero quis dictum maximus, massa sem rutrum risus, non convallis
                lorem mi sed felis. Etiam sapien arcu, finibus at ipsum at, maximus feugiat magna.
                Pellentesque posuere purus non risus aliquam, ac imperdiet nulla porttitor. Aliquam
                vel est quis metus viverra malesuada facilisis eu ex. Mauris aliquet hendrerit
                neque, nec pharetra magna tristique feugiat.
            </p>
            <p>
                Lorem ipsum dolor sit amet, consectetur adipiscing elit. Maecenas tincidunt euismod
                tortor eget eleifend. Mauris porttitor tortor vel neque condimentum commodo. Aliquam
                ornare posuere enim, quis laoreet magna faucibus vitae. Maecenas posuere hendrerit
                lectus, at dignissim magna finibus eu. Sed vulputate euismod ornare. Aenean eleifend
                sed mi quis bibendum. Etiam imperdiet elit dui, et eleifend elit auctor molestie.
                Aenean sodales, libero quis dictum maximus, massa sem rutrum risus, non convallis
                lorem mi sed felis. Etiam sapien arcu, finibus at ipsum at, maximus feugiat magna.
                Pellentesque posuere purus non risus aliquam, ac imperdiet nulla porttitor. Aliquam
                vel est quis metus viverra malesuada facilisis eu ex. Mauris aliquet hendrerit
                neque, nec pharetra magna tristique feugiat.
            </p>
        </>
    );
}
