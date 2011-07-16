from Console import Console

if __name__ == '__main__':
    try:
        Console().cmdloop("Hello!\nThis is PMD interactive console. Control your devices easily!")
    except Exception as exc:
        print exc