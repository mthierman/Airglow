export default async (input: string) => {
    try {
        const response = await fetch(input);

        if (!response.ok) {
            throw new Error("Network response was not OK");
        }

        return true;
    } catch (error) {
        return false;
    }
};
