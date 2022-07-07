# UIXSDK
UI/UX Software Develope Kit

## Directories

| Path              | Description                  |
|:------------------|:-----------------------------|
| ${ROOT}           | 프로젝트 최상위 디렉토리.               |
| ${ROOT}/assets    | 튜토리얼 예제에서 사용하는 리소드 데이터 디렉토리. |
| ${ROOT}/packages  | UIXSDK를 구성하는 패키지 프로젝트 디렉토리.  |
| ${ROOT}/tools     | 앱개발에 도움이 되는 도구 프로젝트 디렉토리.    |
| ${ROOT}/tutorials | 주요 패키지의 튜토리얼 프로젝트의 최상위 디렉토리. |

## 주요 프로젝트

| Name      | Path                       | Description                                           | URL                                            |
|-----------|:---------------------------|:------------------------------------------------------|------------------------------------------------|
| Angle     | ${ROOT}/packages/angle     | 구글의 ANGLE Library 프로젝트. Qt에서 배포하는 버전의 바이너리를 사용한다. | https://chromium.googlesource.com/angle/angle/ |
| Assimp    | ${ROOT}/packages/assimp    | Open Asset Import Library.                            | https://assimp-docs.readthedocs.io/en/v5.1.0/  |
| Blend2d   | ${ROOT}/packages/blend2d   | 2D Vector Graphics Engine.                            | https://blend2d.com/                           |
| Bzip2     | ${ROOT}/packages/bzip2     | high-quality data compressor library.                 | https://www.sourceware.org/bzip2/              |
| CURL      | ${ROOT}/packages/curl      | URL의 데이터를 주고 받는 명렁어 도구 및 라이브러리.                       | https://curl.se/                               |
| JsonCpp   | ${ROOT}/packages/jsoncpp   | json 파서 및 생성 라이브러리.                                   | https://github.com/open-source-parsers/jsoncpp |
| SDL       | ${ROOT}/packages/SDL       | Simple Direct media Layer                             | https://www.libsdl.org/                        |
| SDL iMAGE | ${ROOT}/packages/SDL_image | SDL image sub project.                                | https://github.com/libsdl-org/SDL_image        |
| SDL Mixer | ${ROOT}/packages/SDL_mixer | SDL Audio mixer sub project.                          | https://github.com/libsdl-org/SDL_mixer        |
| SDL TTF   | ${ROOT}/packages/SDL_ttf   | SDL font sub project. ( using freetype and harfbuzz ) | https://github.com/libsdl-org/SDL_ttf          |
| xz        | ${ROOT}/packages/xz        | LZMA2 기반의 압축 유틸리티 라이브러리.                              | https://tukaani.org/xz/                        |

### Tutorials
각각의 주요 패키지들의 튜토리얼 프로젝트를 카테고리별로 작성 한다.
지속적으로 추가 될 예정이다.

#### Tutorials - SDL

- http://lazyfoo.net/tutorials/SDL/index.php 를 기반으로 생성 되었다.
- 기본적으로 원본 코드를 기준으로 작성되었으며, 일부 리소스의 파일명을 변경하였다.
- CMake 를 사용하여 빌드 되도록 추가적으로 작업을 하였다.

#### Tutorials - Blend2D

- Blend2D 에 대한 API 학습을 위해서 작성 한다.


