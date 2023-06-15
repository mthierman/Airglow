export default function App() {
    return (
        <div className="flex gap-2 p-2 text-neutral-400">
            <select className="w-min rounded bg-[#00000060]">
                <option className="bg-black" value="typescript">
                    TypeScript
                </option>
                <option className="bg-black" value="playground">
                    TypeScript Playground
                </option>
                <option className="bg-black" value="google">
                    Google
                </option>
            </select>
            <input
                name="myInput"
                type="text"
                className="w-full rounded bg-[#00000060] p-1 text-center outline-none focus:outline-none"
            />
            <select className="w-min rounded bg-[#00000060]">
                <option className="bg-black" value="typescript">
                    TypeScript
                </option>
                <option className="bg-black" value="playground">
                    TypeScript Playground
                </option>
                <option className="bg-black" value="google">
                    Google
                </option>
            </select>
            <input
                name="myInput2"
                type="text"
                className="w-full rounded bg-[#00000060] p-1 text-center outline-none focus:outline-none"
            />
        </div>
    );
}
