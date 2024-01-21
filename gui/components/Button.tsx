export interface ButtonProps {
    text?: string;
}

export const Button = ({ text = "" }: ButtonProps) => {
    return (
        <button className="button">
            {text}
        </button>
    );
};
