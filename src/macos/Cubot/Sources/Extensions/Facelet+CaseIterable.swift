import Cubot

extension cubot.Cube.Facelet: CaseIterable {
    public static var allCases: [cubot.Cube.Facelet] = [
        .up, .down, .right, .left, .front, .back,
    ]
}
