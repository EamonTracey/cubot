import Cubot
import SwiftUI

@main
struct CubotApp: App {
    @State private var bgColor = Color.red
    var body: some Scene {
        WindowGroup {
            MainView()
        }
    }
}
