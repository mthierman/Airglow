export default function App() {
    return (
        <div className="h-full w-full rounded p-8">
            <div className="h-full w-full rounded-xl bg-[#00000040] text-neutral-400 shadow-2xl shadow-neutral-700">
                <div className="flex flex-col gap-4 p-4">
                    <div className="text-2xl text-neutral-400">Settings</div>
                    <label className="text-2x flex items-center">
                        <span className="w-full whitespace-nowrap">
                            Homepage: Main Panel
                        </span>
                        <input
                            className="w-full rounded-xl border-2 border-neutral-800 bg-black p-2"
                            type="url"
                            name="homepage"
                            defaultValue="https://www.google.com/"
                        />
                    </label>
                    <label className="text-2x flex items-center">
                        <span className="w-full whitespace-nowrap">
                            Homepage: Side Panel
                        </span>
                        <input
                            className="w-full rounded-xl border-2 border-neutral-800 bg-black p-2"
                            type="url"
                            name="homepage"
                            defaultValue="https://www.bing.com/"
                        />
                    </label>
                </div>
            </div>
        </div>
    );
}
