export interface ButtonProps {
    text?: string;
}

export const Button = ({ text = "Default" }: ButtonProps) => {
    return <button>{text}</button>;
};
