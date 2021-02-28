String str_address;
class Mac
{
public:
    uint8_t arr_address[6];
    Mac()
    {
    }
    Mac(uint8_t addr[])
    {
        setAddress(addr);
    }
    uint8_t* getAddress(){
        return this->arr_address;
    }
    String getStrAddress(){
        char src[6];
        str_address = "";
        for (int i = 0; i < 6; i++)
        {   
            sprintf(src, "%02X", arr_address[i]);
            str_address += String(src);
            if (i < 5)
            {
                sprintf(src, ":");
                str_address += String(src);
            }
        }
        return str_address;
    }
    int CharToNum(char c)
    {
        if (c >= 'A' && c <= 'F')
            return c - 'A' + 10;
        else if (c >= 'a' && c <= 'f')
            return c - 'a' + 10;
        else if (c >= '0' && c <= '9')
            return c - '0';
        return -1;
    }
    int HexStringToNumber(String hex)
    {
        int number = 0;
        int multiple = 1;
        int temp;
        // Serial.print(hex+"  " );
        for (int i = hex.length() - 1; i >= 0; i--)
        {
            temp = CharToNum(hex[i]) * multiple;
            if (temp == -1)
            {
                return -1;
            }
            number += CharToNum(hex[i]) * multiple;
            multiple *= 16;
        }
        return number;
    }

    bool StringToHex(String adr)
    {
        String hex_str = adr;
        hex_str += ":";
        int cnt = 0;
        String t = "";
        int temp;
        for (int i = 0; i < hex_str.length(); i++)
        {
            if (hex_str[i] == ':')
            {
                temp = HexStringToNumber(t);
                if (temp == -1)
                {
                    return -1;
                }
                arr_address[cnt++] = temp;
                t = "";
                continue;
            }
            t += hex_str[i];
        }
    }

    Mac(String address)
    {
        setAddress(address);
    }

    void setAddress(uint8_t addr[])
    {
        for(int i = 0; i < 6; i++)
        {
            this->arr_address[i] = addr[i];
        }
        
    }
    void setAddress(String address)
    {
        StringToHex(address);
    }

    void copyConstantUint(const uint8_t *copy){
        for(int i = 0 ; i < 6 ; i ++){
            arr_address[i] = copy[i];
        }
    }
};