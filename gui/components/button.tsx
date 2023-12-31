export interface ButtonProps {
    text?: string;
}

export const Button = ({ text = "Default" }: ButtonProps) => {
    return (
        <button className="grid rounded-md bg-slate-50 px-4 py-2 text-sky-500 shadow-2xl shadow-slate-300 hover:bg-slate-900 active:bg-slate-800 dark:bg-slate-950 dark:shadow-slate-950">
            {text}
        </button>
    );
};
