export default function App() {
    return (
        <div className="flex gap-2 p-2">
            <select className="w-min rounded bg-neutral-800 text-neutral-500">
                <option value="typescript">TypeScript</option>
                <option value="playground">TypeScript Playground</option>
                <option value="google">Google</option>
            </select>
            <input
                name="myInput"
                type="text"
                className="w-full rounded bg-neutral-800 p-1 text-center text-white outline-none focus:outline-none"
            />
            <select className="w-min rounded bg-neutral-800 text-neutral-500">
                <option value="typescript">TypeScript</option>
                <option value="playground">TypeScript Playground</option>
                <option value="google">Google</option>
            </select>
            <input
                name="myInput2"
                type="text"
                className="w-full rounded bg-neutral-800 p-1 text-center text-white outline-none focus:outline-none"
            />
        </div>
    );
}
