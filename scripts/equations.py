import functools
import inspect
import typing
from parsec import *


def trim_whitespace(parser):
    @generate
    def fn():
        yield many(spaces())
        result = yield parser
        yield many(spaces())
        return result
    return fn


@generate
def sign():
    token = yield one_of('+-')
    return token


@trim_whitespace
@generate
def integer():
    sign_token = yield optional(sign, '')
    tokens = yield many1(digit())
    text = sign_token + ''.join(tokens)
    return {
        "value": int(text),
        "text": text
    }


# @trim_whitespace
@generate
def variable():
    tokens = yield


# @trim_whitespace
@generate
def expr():
    yield spaces()
    value = yield integer ^ variable
    return {
        "value": value,
        "text": value["text"]
    }



print(expr.parse_strict("1"))
