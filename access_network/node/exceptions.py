
class DbError(Exception):
    def __init__(self, value):
        self.value = value

    def __str__(self):
        return repr(self.value)

class LogSyncError(Exception):
    def __init__(self, value):
        self.value = value

    def __str__(self):
        return repr(self.value)
